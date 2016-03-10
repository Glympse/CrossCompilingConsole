//------------------------------------------------------------------------------
//
// Copyright (c) 2015 Glympse Inc.  All rights reserved.
//
//------------------------------------------------------------------------------

#ifndef LATLNG_H__GLYMPSE__
#define LATLNG_H__GLYMPSE__

namespace Glympse
{
    
class LatLng : public Common< ILatLng >
{
    public: double _latitude;
        
    public: double _longitude;

    public: LatLng()
    {
        _latitude = Double::NaN;
        _longitude = Double::NaN;
    }
    
    public: LatLng(double latitude, double longitude)
    {
        _latitude = latitude;
        _longitude = longitude;
    }
    
    /**
     * ILatLng section
     */
        
    public: virtual bool hasLocation()
    {
        return Location::isValid(_latitude, _longitude);
    }
    
    public: virtual double getLatitude()
    {
        return _latitude;
    }
        
    public: virtual double getLongitude()
    {
        return _longitude;
    }
        
    public: virtual int32 getLatitudeE6()
    {
        return (int32)( _latitude * (double)1e6 );
    }
        
    public: virtual int32 getLongitudeE6()
    {
        return (int32)( _longitude * (double)1e6 );
    }
    
    /**
     * ICommon section
     */
        
    public: virtual /*S*override**/ bool equals(const GCommonObj& o)
    {
        O<LatLng> l = (O<LatLng>)o;
        if ( NULL == l )
        {
            return false;
        }
            
        return
            ( l->getLatitudeE6() == getLatitudeE6() ) &&
            ( l->getLongitudeE6() == getLongitudeE6() );
    }
        
    public: virtual /*S*override**/ int32 hashCode()
    {
        O<HashCodeBuilder> builder = new HashCodeBuilder();
        builder->append(getLatitudeE6());
        builder->append(getLongitudeE6());
        return builder->toHashCode();
    }
};
    
}

#endif // !LATLNG_H__GLYMPSE__
