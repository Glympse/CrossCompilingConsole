package com.glympse.android.core;

import java.util.Hashtable;

import android.content.Context;

import com.glympse.android.hal.HalFactory;
import com.glympse.android.lib.LibFactory;
import com.glympse.android.lib.Location;
import com.glympse.android.lib.Primitive;
import com.glympse.android.hal.GHashtable;
import com.glympse.android.hal.Platform;
import com.glympse.android.hal.GVector;
import com.glympse.android.hal.GLinkedList;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;

/**
 * The CoreFactory class provides factory methods for constructing all API primitives.
 */
public abstract class CoreFactory
{
    /**
     * Constructs GDrawable from content URI or a path to a graphic file.
     * The method synchronously reads specified file and creates in memory image representation.
     * Rotation argument is only used, when image is decoded back (before uploading or caching).
     *
     * @note It is not recommended to call this method from UI thread as it loads (and potentially resizes)
     * image synchronously. It is preferable to perform all I/O operations from background.
     *
     * @param resource Content URI or a path to a file that contains the graphic.
     * @param rotation The degrees of clockwise rotation that should be applied to the image.
     * @return Drawable object or null if loading fails.
     */
    public static GDrawable createDrawable(String resource, int rotation)
    {
        return HalFactory.createDrawable(resource, rotation);
    }

    /**
     * Constructs uninitialized primitive object of specific type.
     * It might be useful for array and object primitives
     * (CC::PRIMITIVE_TYPE_ARRAY, CC::PRIMITIVE_TYPE_OBJECT).
     *
     * @param type Primitive type. See CC for details.
     */
    public static GPrimitive createPrimitive(int type)
    {
        return new Primitive(type);
    }

    /**
     * Constructs initialized primitive object of CC::PRIMITIVE_TYPE_DOUBLE type.
     */
    public static GPrimitive createPrimitive(double value)
    {
        return new Primitive(value);
    }

    /**
     * Constructs initialized primitive object of CC::PRIMITIVE_TYPE_LONG type.
     */
    public static GPrimitive createPrimitive(long value)
    {
        return new Primitive(value);
    }

    /**
     * Constructs initialized primitive object of CC::PRIMITIVE_TYPE_BOOL type.
     */
    public static GPrimitive createPrimitive(boolean value)
    {
        return new Primitive(value);
    }

    /**
     * Constructs initialized primitive object of CC::PRIMITIVE_TYPE_STRING type.
     */
    public static GPrimitive createPrimitive(String value)
    {
        return new Primitive(value);
    }

    /**
     * Constructs initialized primitive object of CC::PRIMITIVE_TYPE_NULL type.
     */
    public static GPrimitive createPrimitive()
    {
        return new Primitive();
    }

    /**
     * Creates new location object from location data.
     *
     * time, latitude and longitude fields must be specified. Any other field can be set to Float.NaN.
     *
     * @param time Location timestamp (milliseconds since 1/1/1970). It is preferable to use server time here.
     * @param latitude Location latitude in degrees.
     * @param longitude Location longitude in degrees.
     * @param speed Location speed in meters per second over ground. Speed must be positive.
     * @param bearing Location bearing in degrees. Bearing is the horizontal direction of travel of this device,
     * and is not related to the device orientation. The input must be in range [0.0, 360.0].
     * @param altitude Location altitude in meters above sea level.
     * @param haccuracy Horizontal accuracy of this location in meters.
     * @param vaccuracy Vertical accuracy of this location in meters.
     */
    public static GLocation createLocation(long time, double latitude, double longitude,
        float speed, float bearing, float altitude, float haccuracy, float vaccuracy)
    {
        return new Location(time, latitude, longitude, speed, bearing, altitude, haccuracy, vaccuracy);
    }

    /**
     * Creates new location object from location data.
     *
     * @param latitude Location latitude in degrees.
     * @param longitude Location longitude in degrees.
     */
    public static GLocation createLocation(double latitude, double longitude)
    {
        return new Location(0, latitude, longitude, Float.NaN, Float.NaN, Float.NaN, Float.NaN, Float.NaN);
    }

    /**
     * Creates new region object.
     *
     * @param latitude The latitude of the central point of the alert region.
     * @param longitude The longitude of the central point of the alert region.
     * @param radius The radius of the central point of the alert region, in meters.
     * @param rid Unique identifier of a region.
     * @return Region object.
     */
    public static GRegion createRegion(double latitude, double longitude, double radius, String rid)
    {
        return LibFactory.createRegion(latitude, longitude, radius, rid);
    }

    /**
     * Returns client operating system name. e.g. "ios" or "android"
     */
    public static String getOsName()
    {
        return Platform.getOsName();
    }
}
