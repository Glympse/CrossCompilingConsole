//------------------------------------------------------------------------------
//
// Copyright (c) 2014 Glympse Inc.  All rights reserved.
//
//------------------------------------------------------------------------------

package com.glympse.android.core;

import com.glympse.android.hal.Helpers;
import com.glympse.android.lib.HttpJob;
import com.glympse.android.lib.json.*;

/**
 * Basic serialization tools.
 */
public class CoreTools
{
    private CoreTools()
    {
    }

    /**
     * Converts GPrimitive into string representation.
     */
    public static String primitiveToString(GPrimitive data)
    {
        return JsonSerializer.toString(data);
    }

    /**
     * Parses JSON string into GPrimitive object.
     */
    public static GPrimitive stringToPrimitive(String json)
    {
        return JsonSerializer.toPrimitive(json);
    }

    /**
     * Convert http method enum to string.
     */
    public static String httpMethodEnumToString(int methodType)
    {
        return HttpJob.httpMethodEnumToString(methodType);
    }

    /**
     * Convert http method string to enum.
     */
    public static int httpMethodStringToEnum(String methodString)
    {
        return HttpJob.httpMethodStringToEnum(methodString);
    }

    /**
     * Translates string into URL-encoded format.
     */
    public static String urlEncode(String str)
    {
        return Helpers.urlEncode(str);
    }

    /**
     * Decodes a URL-encoded string.
     */
    public static String urlDecode(String str)
    {
        return Helpers.urlDecode(str);
    }
};



