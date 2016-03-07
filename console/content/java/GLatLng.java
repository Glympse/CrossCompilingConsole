//------------------------------------------------------------------------------
//
// Copyright (c) 2012 Glympse Inc.  All rights reserved.
//
//------------------------------------------------------------------------------

package com.glympse.android.core;

import java.util.*;
import com.glympse.android.api.*;
import com.glympse.android.core.*;
import com.glympse.android.lib.*;
import com.glympse.android.lib.json.*;
import com.glympse.android.hal.*;

/**
 * Specifies a single location point (latitude and longitude).
 */
/*O*/public/**/ interface GLatLng extends GCommon
{
    public boolean hasLocation();

    public double getLatitude();

    public double getLongitude();

    public int getLatitudeE6();

    public int getLongitudeE6();
};

/*C*typedef GLatLng  GLatLng;**/



