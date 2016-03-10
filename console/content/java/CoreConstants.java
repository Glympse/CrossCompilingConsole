//------------------------------------------------------------------------------
//
// Copyright (c) Glympse Inc.  All rights reserved.
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
 * Basic enumerations belonging to Core layer.
 */
/*O*/public/**/ class CoreConstants
{
    /**
     * @name Primitive Types
     *
     * Enumeration of all types supported by IPrimitive object.
     */

    public static final int PRIMITIVE_TYPE_ARRAY         = 0x00000001;
    public static final int PRIMITIVE_TYPE_OBJECT        = 0x00000002;
    public static final int PRIMITIVE_TYPE_DOUBLE        = 0x00000004;
    public static final int PRIMITIVE_TYPE_LONG          = 0x00000008;
    public static final int PRIMITIVE_TYPE_BOOLEAN       = 0x00000010;
    public static final int PRIMITIVE_TYPE_STRING        = 0x00000020;
    public static final int PRIMITIVE_TYPE_NULL          = 0x00000040;

    /**
     * @name Location Provider States
     *
     * Location provider changes its internal state between one of these values.
     */

    public static final int LOCATION_STATE_NOT_DETERMINED  = 1;
    public static final int LOCATION_STATE_DENIED          = 2;
    public static final int LOCATION_STATE_ACQUIRED        = 3;
    public static final int LOCATION_STATE_ERROR           = 4;

    /**
     * @name Log Levels
     *
     * Log level specifies the level of details of log messages.
     * Log levels are ordered from the most informative INFO to the least informative CRITICAL.
     * Log levels are applied through IGlympse#overrideLoggingLevels().
     */

    public static final int INFO         = 1;
    public static final int DUMP         = 2;
    public static final int NOTICE       = 3;
    public static final int WARNING      = 4;
    public static final int ERROR        = 5;
    public static final int CRITICAL     = 6;
    public static final int NONE         = 7;

    /**
     * @name Drawable Scale Types
     *
     * Scale type defines the way image gets shrunk when resized.
     */

    /**
     * SCALE_EXACT means to scale the image to exactly the specified size.
     * If the user passed in SCALE_EXACT, or the image size is already exactly
     * what the caller is asking for, then do nothing.
     */
    public static final int DRAWABLE_SCALE_EXACT           = 0;

    /**
     * SCALE_FIT_OR_SMALLER is similar to SCALE_FIT, except that if the
     * image is already smaller than the width *and* height passed in, then
     * we don't do any scaling at all.
     */
    public static final int DRAWABLE_SCALE_FIT_OR_SMALLER  = 1;

    /**
     * SCALE_FIT means to scale the image up or down so that the image
     * perfectly fits within the width/height while keeping the aspect ratio.
     * Either the new width will exactly match the width passed in and the
     * the new height will be <= the height passed in, or vice versa.
     */
    public static final int DRAWABLE_SCALE_FIT             = 2;

    /**
     * SCALE_FILL_OR_SMALLER is similar to SCALE_FILL, except that if the
     * image is already smaller than the width *or* height passed in, then
     * we don't do any scaling at all.
     */
    public static final int DRAWABLE_SCALE_FILL_OR_SMALLER = 3;

    /**
     * SCALE_FILL means to scale the image up or down so that the image
     * completely fills the entire width/height while keeping the aspect ratio.
     * Either the new width will exactly match the width passed in and the
     * the new height will be >= the height passed in, or vice versa.
     */
    public static final int DRAWABLE_SCALE_FILL            = 4;

    /**
     * @name Drawable Compression Types
     *
     * Compression type defines the format used for image compression.
     */

    /**
     * JPEG compression method is used.
     */
    public static final int DRAWABLE_COMPRESS_JPG = 0;

    /**
     * PNG compression method is used.
     */
    public static final int DRAWABLE_COMPRESS_PNG = 1;

    /**
     * @name Device Activities
     *
     * The detected activity of the device.
     */

    /**
     * Device activity cannot be recognized.
     */
    public static final int ACTIVITY_UNKNOWN         = 0;

    /**
     * The device is in a vehicle, such as a car.
     */
    public static final int ACTIVITY_IN_VEHICLE      = 1;

    /**
     * The device is on a bicycle.
     */
    public static final int ACTIVITY_ON_BICYCLE      = 2;

    /**
     * The device is on a user who is walking or running.
     */
    public static final int ACTIVITY_ON_FOOT         = 3;

    /**
     * The device is still (not moving).
     */
    public static final int ACTIVITY_STILL           = 4;

    /**
     * The device angle relative to gravity changed significantly.
     */
    public static final int ACTIVITY_TILTING         = 5;

    /**
     * @name Http Methods
     *
     * Valid HTTP methods that can be used.
     */

    /**
     * Use whatever default method is preferred.
     */
    public static final int HTTP_METHOD_DEFAULT = 0;

    /**
     * Use HTTP GET method.
     */
    public static final int HTTP_METHOD_GET     = 1;

    /**
     * Use HTTP POST method.
     */
    public static final int HTTP_METHOD_POST     = 2;

    /**
     * Use HTTP PUT method.
     */
    public static final int HTTP_METHOD_PUT      = 3;

    /**
     * Use HTTP DELETE method.
     */
    public static final int HTTP_METHOD_DELETE   = 4;
};



