/*
 * Basic class for GPS coordinates helper math 
 * Copyright (C) 2017 TerraClear, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CREATED BY: Koos du Preez - koos@terraclear.com
*/

#include "navmath.h"

namespace  terraclear
{

    float navmath::RadToDeg (float Radians)
    {
        return Radians / ( M_PI / 180 );

    }

    float navmath::DegToRad (float Degrees)
    {
        return Degrees * ( M_PI / 180 );

    }

    float navmath::FeetToMiles (float Feet)
    {
        return Feet / FeetPerMile;
    }

    float navmath::MilesToFeet (float Miles)
    {
        return Miles * FeetPerMile;
    }

    float navmath::FeetToMeters (float Feet)
    {
        return Feet / FeetPerMeter;
    }

    float navmath::MetersToFeet (float Meters)
    {
        return Meters * FeetPerMeter;
    }

    float navmath::CalcDistanceStartToEndMiles(navmath::GPSPosition PosStart, navmath::GPSPosition PosEnd)
    {
        //Calculate Distance from A to B in Statute Miles..

        //Using haversine for good accuracy

        float LatDiffRad = DegToRad(PosEnd.LatitudeDegrees - PosStart.LatitudeDegrees);
        float LonDiffRad = DegToRad(PosEnd.LongitudeDegrees - PosStart.LongitudeDegrees);

        float a =  sin(LatDiffRad / 2) * sin(LatDiffRad / 2) + sin(LonDiffRad / 2) *
                    sin(LonDiffRad/2) * cos(DegToRad(PosStart.LatitudeDegrees)) * cos(DegToRad(PosEnd.LatitudeDegrees));

        float c = 2 * atan2(sqrt(a), sqrt(1-a));

        float DistanceMiles = EarthRaduisMiles * c;

        return DistanceMiles;
    }

    float navmath::CalcBearingDegrees(GPSPosition PosStart, GPSPosition PosEnd)
    {
        //Bearing from A to B (Forward Azimuth)

        float LonDiffRad = DegToRad(PosEnd.LongitudeDegrees - PosStart.LongitudeDegrees);

        float y = sin(LonDiffRad) * cos(DegToRad(PosEnd.LatitudeDegrees));
        float x =  cos(DegToRad(PosStart.LatitudeDegrees)) * sin(DegToRad(PosEnd.LatitudeDegrees)) -
                    sin(DegToRad(PosStart.LatitudeDegrees)) * cos(DegToRad(PosEnd.LatitudeDegrees)) * cos(LonDiffRad);

        float b =  RadToDeg(atan2(y, x));

        float BearingDegrees = (b < 0)? 360+b : b; //Bearing in degrees right of 0

        return BearingDegrees;
    }

    float navmath::CalcTimeOfArrivalSeconds(float SpeedMph, float DistanceMiles)
    {
        float TimeOfArrivalSeconds = 0;

        if ((SpeedMph > 0) && (DistanceMiles > 0))
        {
           TimeOfArrivalSeconds = DistanceMiles / (SpeedMph / 60 / 60);
        }
        else
            TimeOfArrivalSeconds = 0;

        return TimeOfArrivalSeconds;
    }

    float navmath::CalcDistanceTraveledMiles(float SpeedMph, float TimeElapsedSeconds)
    {
        float DistanceTraveledMiles = 0;
         if (SpeedMph > 0)
         {
             //Calculate distance based on speed and elapsed time.
             DistanceTraveledMiles = TimeElapsedSeconds * SpeedMph / 60 / 60;
         }
        else
            DistanceTraveledMiles = 0;

        return DistanceTraveledMiles;
    }


    navmath::GPSPosition navmath::CalcPosMidpoint(navmath::GPSPosition PosStart, navmath::GPSPosition PosEnd)
    {

        //Calculate the mid point between two coordinates.

        float LonDiffRad = DegToRad(PosEnd.LongitudeDegrees - PosStart.LongitudeDegrees);

        float Bx = cos(DegToRad(PosEnd.LatitudeDegrees)) * cos(LonDiffRad);
        float By = cos(DegToRad(PosEnd.LatitudeDegrees)) * sin(LonDiffRad);

        float MidLatRad = atan2(sin(DegToRad(PosStart.LatitudeDegrees)) +
                           sin(DegToRad(PosEnd.LatitudeDegrees)), sqrt((cos(DegToRad(PosStart.LatitudeDegrees)) + Bx) *
                           (cos(DegToRad(PosStart.LatitudeDegrees)) + Bx) + By * By ));
        float MidLonRad = DegToRad(PosStart.LongitudeDegrees) + atan2(By, cos(DegToRad(PosStart.LatitudeDegrees)) + Bx);

        navmath::GPSPosition PosMidPoint;
        PosMidPoint.LatitudeDegrees = RadToDeg(MidLatRad);
        PosMidPoint.LongitudeDegrees = RadToDeg(MidLonRad);

        return PosMidPoint;
    }

    navmath::GPSPosition navmath::CalcPosEnd(navmath::GPSPosition PosStart, float BearingDegrees, float DistanceMiles)
    {
        //Destination from A based on Azimuth and Distance
        float BearingRad = DegToRad(BearingDegrees);

        float DestLatRad =  asin( sin(DegToRad(PosStart.LatitudeDegrees)) * cos(DistanceMiles / EarthRaduisMiles) +
                                   cos(DegToRad(PosStart.LatitudeDegrees)) * sin(DistanceMiles / EarthRaduisMiles) * cos(BearingRad) );

        float DestLonRad = DegToRad(PosStart.LongitudeDegrees) + atan2(sin(BearingRad) * sin(DistanceMiles / EarthRaduisMiles) *
                            cos(DegToRad(PosStart.LatitudeDegrees)), cos(DistanceMiles / EarthRaduisMiles) -
                            sin(DegToRad(PosStart.LatitudeDegrees)) * sin(DestLatRad));

        navmath::GPSPosition PosEnd;
        PosEnd.LatitudeDegrees = RadToDeg(DestLatRad);
        PosEnd.LongitudeDegrees = RadToDeg(DestLonRad);

        return PosEnd;
    }

}

/*
 function get_gps_distance($lat1,$long1,$d,$angle)
{
    # Earth Radious in KM
    $R = 6378.14;

    # Degree to Radian
    $latitude1 = $lat1 * (M_PI/180);
    $longitude1 = $long1 * (M_PI/180);
    $brng = $angle * (M_PI/180);

    $latitude2 = asin(sin($latitude1)*cos($d/$R) + cos($latitude1)*sin($d/$R)*cos($brng));
    $longitude2 = $longitude1 + atan2(sin($brng)*sin($d/$R)*cos($latitude1),cos($d/$R)-sin($latitude1)*sin($latitude2));

    # back to degrees
    $latitude2 = $latitude2 * (180/M_PI);
    $longitude2 = $longitude2 * (180/M_PI);

    # 6 decimal for Leaflet and other system compatibility
   $lat2 = round ($latitude2,6);
   $long2 = round ($longitude2,6);

   // Push in array and get back
   $tab[0] = $lat2;
   $tab[1] = $long2;
   return $tab;
 }
 */