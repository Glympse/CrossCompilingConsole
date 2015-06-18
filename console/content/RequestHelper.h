//------------------------------------------------------------------------------
//
// Copyright (c) 2013 Glympse Inc.  All rights reserved.
//
//------------------------------------------------------------------------------

#ifndef REQUESTHELPER_H__TOOLBOX__GLYMPSE__
#define REQUESTHELPER_H__TOOLBOX__GLYMPSE__

namespace Glympse
{
namespace Toolbox
{

/**
 * Simplifies handling of incoming and outgoing Glympse requests in deep messaging integration scenario.
 *
 * @note The helper can be reused to send multiple requests.
 * @note detach() method must be called explicitly.
 */
/*O*public**/ class RequestHelper : public Common< IEventListener >
{
    /**
     * Notifies consumer of this class on some events related to Glympse requests. 
     */
    public: struct IRequestListener : public ICommon
    {
        /**
         * The method is called as a result of calling RequestHelper.sendRequest(),
         * when Glympse API is done with all preparations and request invite URL is finally generated.
         *   
         * @param url URL to be sent to requestee. 
         * @param tag Custom object provided to RequestHelper.sendRequest() to identify the request. 
         */
        public: virtual void requestIsReadyToBeSent(const GString& url, const GCommonObj& tag) = 0;
        
        /**
         * The method is called, when platform detects incoming Glympse request.
         * User confirmation is required before ticket can be sent back to requester.
         * 
         * This method is called as a result of calling GGlympse.decodeInvite() or GGlympse.openUrl().
         * Both methods allow to associate a piece of meta data, that is propagated all way through
         * the library back to host application and allows to associate original message or code supplied
         * to one of the methods above with resulting GE.PLATFORM_INVITE_REQUEST event. 
         * 
         * At this point ticket is ready to be sent using GGlympse.sendTicket().
         * Proper handling for the delivery of LINK invites should be added though.
         *  
         * @param from Glympse user, who initiated the request. 
         * @param ticket Ticket to be sent in response to Glympse request.
         * @param invite Invite object passed to GGlympse.decodeInvite() or GGlympse.openUrl().
         */
        public: virtual void requestIsReceived(const GUser& from, const GTicket& ticket, const GInvite& invite) = 0;
    };
    
    /**
     * @name Members
     */    
    
    /**
     * Instance of Glympse platform. 
     */
    private: GGlympse _glympse;
    
    /**
     * Listener object provided by host application. 
     */
    private: O<IRequestListener> _listener;
    
    /**
     * @name Internal constants
     */      
    
    private: static const int32 REQUEST_OBJECT_TAG = 0x1;
    
    /**
     * @name Public tools
     */    
    
    /**
     * Initializes the helper.
     * 
     * @param glympse Instance of Glympse platform. 
     * @param listener Back channel to notify host application on helper's activity.
     */
    public: void initialize(const GGlympse& glympse, const O<IRequestListener>& listener)
    {
        if ( ( NULL != _glympse ) || ( NULL == glympse ) || ( NULL == listener ) )
        {
            // Already initialized or invalid arguments. 
            return;
        }
        
        // Remember platform and listener.
        _glympse = glympse;
        _listener = listener;
        
        // Subscribe on platform events. 
        _glympse->addListener(Object::fromThis(this));
    }
    
    /**
     * Tears down the component. 
     */
    public: void detach()
    {
        if ( NULL == _glympse )
        {
            // Not yet initialized. 
            return;
        }
        
        // Unsubscribe from platform events.
        _glympse->removeListener(Object::fromThis(this));
        
        // Forget platform and listener. 
        _glympse = NULL;
        _listener = NULL;        
    }
    
    /**
     * Prepares Glympse request to be sent. IRequestListener.requestIsReadyToBeSent() 
     * is called, when all preparations are complete and request is ready to be delivered. 
     * 
     * @param selfMdn MDN of request sender. This property is required to provide Glympse 
     * application with a chance to reply to the request. 
     * @param duration Proposed ticket duration. 
     * @param tag Arbitrary object provided by host application to identify the request. 
     */
    public: void sendRequest(const GString& selfMdn, int32 duration, const GCommonObj& tag)
    {
        if ( NULL == _glympse )
        {
            // Not yet initialized. 
            return;
        }        
        
        // Create invite, which represents self user.
        // This invite will be used to send ticket to requester. 
        GInvite selfInvite = GlympseFactory::createInvite(GC::INVITE_TYPE_SMS, NULL, selfMdn);

        // Create ticket object.
        GTicket request = GlympseFactory::createTicket(duration, NULL, NULL);
        request->addInvite(selfInvite);

        if ( NULL != tag )
        {
            // Associate some context with the request object so that we can identify it later.
            request->associateContext(REQUEST_OBJECT_TAG, tag);
        }

        // Create invite, which represents recipient of the request. 
        // This invite will be used to send ticket request.
        GInvite requestInvite = GlympseFactory::createInvite(GC::INVITE_TYPE_LINK, NULL, NULL);
        requestInvite->setRequestTicket(request);

        // Create a container ticket to act as a sink to listen for events on.
        GTicket sink = GlympseFactory::createTicket(0, NULL, NULL);
        sink->addInvite(requestInvite);

        // Subscribe on ticket events to handle custom invite delivery,
        // once request URL is generated by the Glympse API. 
        sink->addListener(Object::fromThis(this));
        
        // Send a request.
        _glympse->requestTicket(sink);
    }
    
    /**
     * @name Internal event handlers
     */    
    
    /**
     * Performs required post processing over incoming request
     * before it can be propagated to host application for user confirmation and delivery. 
     */
    private: void handleIncomingRequest(const GUserTicket& request)
    {
        // Extract ticket to be sent back and perform some checks. 
        GTicket ticket = request->getTicket();
        if ( NULL == ticket )
        {
            // No ticket is associated with request.   
            return;
        }
        
        // Remove all ticket invites. Messaging client does not need this information, 
        // as it always knows from whom the request was received and reply is going to be sent 
        // to the conversation with the requester.  
        while ( ticket->getInvites()->length() > 0 )
        {
            ticket->deleteInvite(ticket->getInvites()->at(0));
        }
        
        // Provide invite of LINK type. Host application is responsible for
        // delivery of this invite in its own way.
        GInvite invite = GlympseFactory::createInvite(GC::INVITE_TYPE_LINK, NULL, NULL);
        ticket->addInvite(invite);
        
        // Notify listener.
        _listener->requestIsReceived(request->getUser(), ticket, request->getInvite());
    }
    
    /**
     * Propagates prepared outgoing request to host application for delivery.  
     */
    private: void handleOutgoingRequest(const GTicket& request)
    {
        // At this point ticket object is populates with the invite that is supposed to 
        // be delivered to the recipient of the request. We just need to grab invite URL
        // and notify our listener object. 
        
        // Sanity checks.
        if ( 0 == request->getInvites()->length() )
        {
            // No invites are found. 
            return;
        }
        GInvite invite = request->getInvites()->at(0);
        if ( GC::INVITE_STATE_NEEDTOSEND != invite->getState() )
        {
            // Invite is in wrong state. 
            return;
        }
        GString requestInviteUrl = invite->getUrl();
        if ( NULL == requestInviteUrl )
        {
            // Invite URL has not been prepared. 
            return;
        }
        
        // Extract tag associated with the request. 
        GCommonObj tag = request->getContext(REQUEST_OBJECT_TAG);
        
        // Notify listener on success. 
        _listener->requestIsReadyToBeSent(requestInviteUrl, tag);
    }    
    
    /**
     * @name GEventListener section
     */
    
    /**
     * Handle events spread by the platform and ticket objects.
     */
    public: virtual void eventsOccurred(const GGlympse& glympse, int32 listener, int32 events, const GCommonObj& obj)
    {
        // Events spread by the platform. 
        if ( GE::LISTENER_PLATFORM == listener )
        {
            // Request to share location is received. 
            if ( 0 != ( GE::PLATFORM_INVITE_REQUEST & events ) )
            {
                GUserTicket request = (GUserTicket)obj;                
                handleIncomingRequest(request);
            }
            
            // Platform is stopped. This is really a corner case. 
            if ( 0 != ( GE::PLATFORM_STOPPED & events ) )
            {
                detach();
            }
        }
        
        // Events spread by ticket objects. 
        if ( GE::LISTENER_TICKET == listener )
        {
            // Request invite has been created and is ready to be sent. 
            if ( 0 != ( GE::TICKET_REQUEST_CREATED & events ) )
            {
                GTicket request = (GTicket)obj;
                handleOutgoingRequest(request);
            }
        }
    }
};
    
}
}

#endif // !REQUESTHELPER_H__TOOLBOX__GLYMPSE__
