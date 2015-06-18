//------------------------------------------------------------------------------
//
// Copyright (c) 2013 Glympse Inc.  All rights reserved.
//
//------------------------------------------------------------------------------

#ifndef GROUPCREATOR_H__TOOLBOX__GLYMPSE__
#define GROUPCREATOR_H__TOOLBOX__GLYMPSE__

namespace Glympse
{
namespace Toolbox
{

/** 
 * Creates Glympse groups all day long :) 
 *
 * Group creating (in terms of this helper) consists of the following steps:
 * - sending ticket to the group (phase 1);
 * - adding group for viewing (phase 2);
 * - sending invites to join this group (phase 3).
 *
 * Current implementation dos not care whether user is already sharing with the group or not.
 * New ticket is sent to the group each time, when wizard is initiated.
 *
 * @note The helper is designed to create single group. Subsequent calls to createGroup() fail immediately. 
 * @note detach() method must be called explicitly.
 * @note Ticket shared with the group remains active even in case if the component is teared down.
 * However, it is expired by the compoent in case of any error in the flow. 
 */
/*O*public**/ class GroupCreator : public Common< IEventListener >
{
    /**
     * Notifies host application on some internal events. 
     * The interfce can be extended based on consumer needs. 
     */
    public: struct IGroupListener : public ICommon
    {
        /**
         * Notifies on the status of sharing to a group.
         *
         * @param success Indicates whether sharing started successfully or failed for whatever reason.
         */
        public: virtual void sharingStatus(bool success) = 0;
        
        /**
         * The method is invoked, when platform is done generating invite message.
         * Since now host application is responsible for invite delivery. 
         * The callback is only invoked for specific invite types (link, SMS if not supported, etc.). 
         */
        public: virtual void deliverInvite(const GGroup& group, const GInvite& invite) = 0;
        
        /**
         * Notifies on the status of invite delivery.
         *
         * @note This callback may or may not be fired for invites delivered on application side.
         * Application is notfied on those invites through IGroupListener::deliverInvite().
         *
         * @param invite Invite object that added through GroupCreator::invite().
         * @param success Delivery status (success or failure).
         */
        public: virtual void inviteCompleted(const GInvite& invite, bool success) = 0;
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
    private: O<IGroupListener> _listener;
    
    /**
     * Group name.
     */
    private: GString _name;
    
    /**
     * Invites to join people to the group.
     */
    private: GVector<GCommonObj>::ptr _invites;
    
    /**
     * The list of invites that we already notified listener on.
     */
    private: GVector<GCommonObj>::ptr _notifications;
    
    /**
     * Ticket sent to the group.
     */
    private: GTicket _ticket;
    
    /**
     * The group.
     */
    private: GGroup _group;
    
    /**
     * @name Public tools
     */
    
    /**
     * Initializes the helper.
     *
     * @param glympse Instance of Glympse platform.
     * @param listener Back channel to notify host application on helper's activity.
     */
    public: void initialize(const GGlympse& glympse, const O<IGroupListener>& listener)
    {
        if ( ( NULL != _glympse ) || ( NULL == glympse ) || ( NULL == listener ) || !glympse->isStarted() )
        {
            // Already initialized or invalid arguments.
            return;
        }
        
        // Remember platform and listener.
        _glympse = glympse;
        _listener = listener;
        
        _invites = CoreFactory::createVector();
        _notifications = CoreFactory::createVector();
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
        
        // Forget platform and listener.
        _glympse = NULL;
        _listener = NULL;
        
        _name = NULL;
        _invites = NULL;
        _notifications = NULL;
        if ( NULL != _ticket )
        {
            _ticket->removeListener(Object::fromThis(this));
            _ticket = NULL;
        }
        if ( NULL != _group )
        {
            _group->removeListener(Object::fromThis(this));
            _group = NULL;
        }
    }
    
    /**
     * Send ticket to a group and starts viewing a group once ticket is sent (only upon success). 
     *
     * @param duration Desired amount of time to share with the group for.
     */
    public: bool createGroup(const GString& name, int32 duration)
    {
        // See if platform is attached and started.
        if ( ( NULL == _glympse ) || !_glympse->isStarted() || ( NULL != _name ) )
        {
            return false;
        }
        
        // Remember group name;
        _name = name;
        
        // Check group name.
        GGroupManager groupManager = _glympse->getGroupManager();
        if ( !groupManager->isValidGroup(name) )
        {
            return false;
        }
        
        // New ticket is sent to a group each time, when wizard is initiated.
        _ticket = sendTicket(name, duration);
        if ( NULL == _ticket )
        {
            return false;
        }
        
        // Depending on platform configuration group manager is either going to start viewing
        // the group or not. It is too early to split the flow based on that here. Lets start
        // sharing with the group first and see how it goes. 
        _ticket->addListener(Object::fromThis(this));
        
        return true;
    }
    
    /**
     * Invite others to join the group.
     */
    public: void invite(int32 type, const GString& address)
    {
        GInvite invite = GlympseFactory::createInvite(type, NULL, address);
        if ( NULL == invite )
        {
            return;
        }
        
        // We act depending on availability of group object at the moment. 
        if ( _group == NULL )
        {
            _invites->addElement(invite);
        }
        else
        {
            _group->send(invite, NULL);
        }
    }
    
    /**
     * @name Internal state machine
     */
    
    private: void proceedToPhase2()
    {
        // See if group is already added.
        _group = _glympse->getGroupManager()->findGroupByGroupId(_name);
        if ( NULL != _group )
        {
            // Send invites to join the group.
            proceedToPhase3();
            return;
        }
        
        // Here we go.. We do not really care, whether platform is going to start
        // viewing us automatically or not. It does not harm if we help the platform a bit.
        _group = _glympse->getGroupManager()->viewGroup(_name);
        if ( NULL == _group )
        {
            miscError();            
            return;
        }
        
        // At this point we already know that ticket was successully sent to the group.
        // It is safe to invite others to join it right away (no need to wait for group to become active).
        proceedToPhase3();
    }
    
    private: void proceedToPhase3()
    {
        // Tell listener that sharing to a group started successfully.
        if ( NULL != _listener )
        {
            _listener->sharingStatus(true);
        }
        
        // Subscribe on group events.
        _group->addListener(Object::fromThis(this));
        
        // Send all pending invites (to join the group).
        int32 invitesCount = _invites->length();
        for ( int32 inviteIndex = 0 ; inviteIndex < invitesCount ; ++inviteIndex )
        {
            GInvite invite = (GInvite)_invites->at(inviteIndex);
            _group->send(invite, NULL);
        }
    }
    
    /**
     * @name GEventListener section
     */
    
    /**
     * Handle events spread by the platform and ticket objects.
     */
    public: virtual void eventsOccurred(const GGlympse& glympse, int32 listener, int32 events, const GCommonObj& obj)
    {
        // Events spread by group objects.         
        if ( GE::LISTENER_GROUP == listener )
        {
            // Invite to join a group was created and now needs to be delivered. 
            if ( 0 != ( GE::GROUP_INVITE_CREATED & events ) )
            {
                // Walk through the list of group invites.
                GArray<GInvite>::ptr invites  = _group->getInvites();
                int32 invitesCount = invites->length();
                for ( int32 inviteIndex = 0 ; inviteIndex < invitesCount ; ++inviteIndex )
                {
                    GInvite invite = invites->at(inviteIndex);
                    if ( GC::INVITE_STATE_NEEDTOSEND == invite->getState() )
                    {
                        // Switch invite to GC::INVITE_STATE_CLIENTSENDING state.
                        invite->initiateClientSideSend();
                        
                        // Notify host application on invite being created and ready for delivery.
                        _listener->deliverInvite(_group, invite);
                    }
                }
            }
            else if ( 0 != ( ( GE::GROUP_INVITE_SENT | GE::GROUP_INVITE_FAILED ) & events ) )
            {
                bool allClear = true;
                
                // Walk through the list of group invites.
                GArray<GInvite>::ptr invites  = _group->getInvites();
                int32 invitesCount = invites->length();
                for ( int32 inviteIndex = 0 ; inviteIndex < invitesCount ; ++inviteIndex )
                {
                    GInvite invite = invites->at(inviteIndex);
                    int32 state = invite->getState();
                    switch ( state )
                    {
                        case GC::INVITE_STATE_CLIENTSENDING:
                        case GC::INVITE_STATE_SUCCEEDED:
                        {
                            completeInvite(invite, true);
                            break;
                        }
                        case GC::INVITE_STATE_FAILED_TO_CREATE:
                        case GC::INVITE_STATE_FAILED_TO_SEND:
                        {
                            completeInvite(invite, false);
                            break;
                        }
                        default:
                        {
                            // There are still some invites that are being delivered.
                            allClear = false;
                            break;
                        }
                    }
                }
                
                // Clear both local and group invites as we already processed all of it.
                if ( allClear )
                {
                    _notifications->removeAllElements();
                    _group->clearInvites();
                }
            }
        }
        
        // Events spread by ticket objects.         
        else if ( GE::LISTENER_TICKET == listener )
        {
            // Invite was sent to the group.
            if ( 0 != ( GE::TICKET_INVITE_SENT & events ) )
            {
                // Start viewing the group.
                proceedToPhase2();
                
                // Unsubscribe from ticket events.
                _ticket->removeListener(Object::fromThis(this));
            }
            
            // Invite is failed to be sent to the group.
            else if ( 0 != ( GE::TICKET_INVITE_FAILED & events ) )
            {
                miscError();
            }
        }
    }
    
    /**
     * @name Misc helpers
     */
    
    private: void completeInvite(const GInvite& invite, bool success)
    {
        if ( !_notifications->contains(invite) )
        {
            if ( NULL != _listener )
            {
                _listener->inviteCompleted(invite, success);
            }
            _notifications->addElement(invite);
        }
    }
    
    private: GTicket sendTicket(const GString& name, int32 duration)
    {
        GTicket ticket = GlympseFactory::createTicket(duration, NULL, NULL);
        GInvite invite = GlympseFactory::createPublicInvite(name);
        ticket->addInvite(invite);
        bool success = _glympse->sendTicket(ticket);
        return success ? ticket : NULL;
    }
    
    private: void miscError()
    {
        // Tell listener that we failed to start sharing with a group.
        if ( NULL != _listener )
        {
            _listener->sharingStatus(false);
        }
        
        // Expire ticket.
        if ( NULL != _ticket)
        {
            _ticket->expire();
        }
        
        // Tear down the component.
        detach();
    }
};

}
}

#endif // !GROUPCREATOR_H__TOOLBOX__GLYMPSE__
