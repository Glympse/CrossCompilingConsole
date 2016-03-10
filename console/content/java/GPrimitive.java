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

/*C*interface GPrimitive;
typedef GPrimitive  GPrimitive;**/

/**
 * The IPrimitive interface is designed to operate with free form data structures.
 * The model behind IPrimitive conforms to JavaScript Object Notation (JSON)
 * (see http://www.ietf.org/rfc/rfc4627 for details).
 *
 * Supported primitive types are (see CC):
 * - array (PRIMITIVE_TYPE_ARRAY) - represents array of IPrimitive objects;
 * - object (PRIMITIVE_TYPE_OBJECT) - represents string hastable with value of IPrimitive type;
 * - double (PRIMITIVE_TYPE_DOUBLE) - represents value of primitive double type;
 * - long (PRIMITIVE_TYPE_LONG) - represents value of primitive long type (Glympse::int64);
 * - boolean (PRIMITIVE_TYPE_BOOL) - represents value of primitive boolean type;
 * - string (PRIMITIVE_TYPE_STRING) - represents value of primitive string type (Glympse::IString);
 * - null (PRIMITIVE_TYPE_BOOL) - indicates that object does not contain any value associated with it.
 */
/*O*/public/**/ interface GPrimitive extends GComparable
{
    /**
     * Common properties.
     */

    public int type();

    public boolean isArray();

    public boolean isObject();

    public boolean isDouble();

    public boolean isLong();

    public boolean isBool();

    public boolean isString();

    public boolean isNull();

    public int size();

    public GPrimitive clone();

    /**
     * merge provides a mechanism for combining two primitives within the following rules.
     * If the rules are not met, the merge fails unless the overrideTarget parameter is set
     * to true and then the merge becomes a copy operation.
     *   1) For boolean string, long, double and null types only the same type may be merged in by default.
     *   2) For an array any primitive can be merged into an array and will be appended to the end of an array. Merging two arrays will append clones of each element of the "from" array.
     *   3) For object(hashtable) primitives only another object may be merged with it by default. For each key in the "from" primitive merge attempts to insert it into the target primitive according to these rules if the key already exists. If it doesn't exist, it always inserts a copy.
     */
    public boolean merge(GPrimitive from, boolean overrideTarget);

    /**
     * Value getters.
     */

    public double getDouble();

    public long getLong();

    public boolean getBool();

    public String getString();

    /**
     * Object getters.
     */

    public GPrimitive get(String key);

    public double getDouble(String key);

    public long getLong(String key);

    public boolean getBool(String key);

    public String getString(String key);

    public Enumeration<String> getKeys();

    public boolean hasKey(String key);

    /**
     * Array getters.
     */

    public GArray<GPrimitive> getArray();

    public GPrimitive get(int index);

    public double getDouble(int index);

    public long getLong(int index);

    public boolean getBool(int index);

    public String getString(int index);

    /**
     * Value modifiers.
     */

    public void set(double value);

    public void set(long value);

    public void set(boolean value);

    public void set(String value);

    public void setNull();

    public void setArray();

    public void setObject();

    /**
     * Object modifiers.
     */

    public void put(String key, GPrimitive value);

    public void put(String key, double value);

    public void put(String key, long value);

    public void put(String key, boolean value);

    public void put(String key, String value);

    public void putNull(String key);

    public void remove(String key);

    /**
     * Array modifiers.
     */

    public void put(GPrimitive value);

    public void put(double value);

    public void put(long value);

    public void put(boolean value);

    public void put(String value);

    public void insert(int index, GPrimitive value);

    public void put(int index, GPrimitive value);

    public void put(int index, double value);

    public void put(int index, long value);

    public void put(int index, boolean value);

    public void put(int index, String value);

    public void putNull(int index);

    public void remove(int index);

    public void remove(GPrimitive value);

};



