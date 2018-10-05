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

#ifndef NAVMATH_H
#define NAVMATH_H

#include <math.h>

#define EarthRaduisFeet         20903520
#define EarthRaduisMiles        3959
#define EarthRadiusKilometers   6371
#define FeetPerMile             5280
#define FeetPerMeter            3.28084

namespace  terraclear
{

    class navmath 
    {
        public:

            struct GPSPosition
            {
                float LongitudeDegrees;
                float LatitudeDegrees;
            };

            static float RadToDeg (float Radians);
            static float DegToRad (float Degrees);
            static float FeetToMiles (float Feet);
            static float MilesToFeet (float Miles);
            static float MetersToFeet (float Meters);
            static float FeetToMeters (float Meters);

            static float      CalcDistanceStartToEndMiles(GPSPosition PosStart, GPSPosition PosEnd);
            static float      CalcBearingDegrees(GPSPosition PosStart, GPSPosition PosEnd);
            static float      CalcTimeOfArrivalSeconds(float SpeedMph, float DistanceMiles);
            static float      CalcDistanceTraveledMiles(float SpeedMph, float TimeElapsedSeconds);
            static GPSPosition CalcPosMidpoint(GPSPosition PosStart, GPSPosition PosEnd);
            static GPSPosition CalcPosEnd(GPSPosition PosStart, float BearingDegrees, float DistanceMiles);

    };

}

#endif /* NAVMATH_H */

