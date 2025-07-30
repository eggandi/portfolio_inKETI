/* Automatically generated file - do not edit */
#ifndef _FFASN1_FFASN1_J2735_2016_H
#define _FFASN1_FFASN1_J2735_2016_H

#include "asn1defs.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef int j2735DSRCmsgID;

extern const ASN1CType asn1_type_j2735DSRCmsgID[];

typedef struct j2735MessageFrame {
  j2735DSRCmsgID messageId;
  ASN1OpenType value;
} j2735MessageFrame;


extern const ASN1CType asn1_type_j2735MessageFrame[];

typedef struct j2735MESSAGE_ID_AND_TYPE { /* object class definition */
  ASN1CType id;
  ASN1CType Type;
} j2735MESSAGE_ID_AND_TYPE;


extern const ASN1CType asn1_type_j2735MESSAGE_ID_AND_TYPE[];

typedef int j2735MsgCount;

extern const ASN1CType asn1_type_j2735MsgCount[];

typedef ASN1String j2735TemporaryID;

extern const ASN1CType asn1_type_j2735TemporaryID[];

typedef int j2735DSecond;

extern const ASN1CType asn1_type_j2735DSecond[];

typedef int j2735Latitude;

extern const ASN1CType asn1_type_j2735Latitude[];

typedef int j2735Longitude;

extern const ASN1CType asn1_type_j2735Longitude[];

typedef int j2735Elevation;

extern const ASN1CType asn1_type_j2735Elevation[];

typedef int j2735SemiMajorAxisAccuracy;

extern const ASN1CType asn1_type_j2735SemiMajorAxisAccuracy[];

typedef int j2735SemiMinorAxisAccuracy;

extern const ASN1CType asn1_type_j2735SemiMinorAxisAccuracy[];

typedef int j2735SemiMajorAxisOrientation;

extern const ASN1CType asn1_type_j2735SemiMajorAxisOrientation[];

typedef struct j2735PositionalAccuracy {
  j2735SemiMajorAxisAccuracy semiMajor;
  j2735SemiMinorAxisAccuracy semiMinor;
  j2735SemiMajorAxisOrientation orientation;
} j2735PositionalAccuracy;


extern const ASN1CType asn1_type_j2735PositionalAccuracy[];

typedef enum j2735TransmissionState {
  j2735TransmissionState_neutral,
  j2735TransmissionState_park,
  j2735TransmissionState_forwardGears,
  j2735TransmissionState_reverseGears,
  j2735TransmissionState_reserved1,
  j2735TransmissionState_reserved2,
  j2735TransmissionState_reserved3,
  j2735TransmissionState_unavailable,
} j2735TransmissionState;

extern const ASN1CType asn1_type_j2735TransmissionState[];

typedef int j2735Speed;

extern const ASN1CType asn1_type_j2735Speed[];

typedef int j2735Heading;

extern const ASN1CType asn1_type_j2735Heading[];

typedef int j2735SteeringWheelAngle;

extern const ASN1CType asn1_type_j2735SteeringWheelAngle[];

typedef int j2735Acceleration;

extern const ASN1CType asn1_type_j2735Acceleration[];

typedef int j2735VerticalAcceleration;

extern const ASN1CType asn1_type_j2735VerticalAcceleration[];

typedef int j2735YawRate;

extern const ASN1CType asn1_type_j2735YawRate[];

typedef struct j2735AccelerationSet4Way {
  j2735Acceleration Long;
  j2735Acceleration lat;
  j2735VerticalAcceleration vert;
  j2735YawRate yaw;
} j2735AccelerationSet4Way;


extern const ASN1CType asn1_type_j2735AccelerationSet4Way[];

typedef ASN1BitString j2735BrakeAppliedStatus;

extern const ASN1CType asn1_type_j2735BrakeAppliedStatus[];

typedef enum j2735TractionControlStatus {
  j2735TractionControlStatus_unavailable,
  j2735TractionControlStatus_off,
  j2735TractionControlStatus_on,
  j2735TractionControlStatus_engaged,
} j2735TractionControlStatus;

extern const ASN1CType asn1_type_j2735TractionControlStatus[];

typedef enum j2735AntiLockBrakeStatus {
  j2735AntiLockBrakeStatus_unavailable,
  j2735AntiLockBrakeStatus_off,
  j2735AntiLockBrakeStatus_on,
  j2735AntiLockBrakeStatus_engaged,
} j2735AntiLockBrakeStatus;

extern const ASN1CType asn1_type_j2735AntiLockBrakeStatus[];

typedef enum j2735StabilityControlStatus {
  j2735StabilityControlStatus_unavailable,
  j2735StabilityControlStatus_off,
  j2735StabilityControlStatus_on,
  j2735StabilityControlStatus_engaged,
} j2735StabilityControlStatus;

extern const ASN1CType asn1_type_j2735StabilityControlStatus[];

typedef enum j2735BrakeBoostApplied {
  j2735BrakeBoostApplied_unavailable,
  j2735BrakeBoostApplied_off,
  j2735BrakeBoostApplied_on,
} j2735BrakeBoostApplied;

extern const ASN1CType asn1_type_j2735BrakeBoostApplied[];

typedef enum j2735AuxiliaryBrakeStatus {
  j2735AuxiliaryBrakeStatus_unavailable,
  j2735AuxiliaryBrakeStatus_off,
  j2735AuxiliaryBrakeStatus_on,
  j2735AuxiliaryBrakeStatus_reserved,
} j2735AuxiliaryBrakeStatus;

extern const ASN1CType asn1_type_j2735AuxiliaryBrakeStatus[];

typedef struct j2735BrakeSystemStatus {
  j2735BrakeAppliedStatus wheelBrakes;
  j2735TractionControlStatus traction;
  j2735AntiLockBrakeStatus albs;
  j2735StabilityControlStatus scs;
  j2735BrakeBoostApplied brakeBoost;
  j2735AuxiliaryBrakeStatus auxBrakes;
} j2735BrakeSystemStatus;


extern const ASN1CType asn1_type_j2735BrakeSystemStatus[];

typedef int j2735VehicleWidth;

extern const ASN1CType asn1_type_j2735VehicleWidth[];

typedef int j2735VehicleLength;

extern const ASN1CType asn1_type_j2735VehicleLength[];

typedef struct j2735VehicleSize {
  j2735VehicleWidth width;
  j2735VehicleLength length;
} j2735VehicleSize;


extern const ASN1CType asn1_type_j2735VehicleSize[];

typedef struct j2735BSMcoreData {
  j2735MsgCount msgCnt;
  j2735TemporaryID id;
  j2735DSecond secMark;
  j2735Latitude lat;
  j2735Longitude Long;
  j2735Elevation elev;
  j2735PositionalAccuracy accuracy;
  j2735TransmissionState transmission;
  j2735Speed speed;
  j2735Heading heading;
  j2735SteeringWheelAngle angle;
  j2735AccelerationSet4Way accelSet;
  j2735BrakeSystemStatus brakes;
  j2735VehicleSize size;
} j2735BSMcoreData;


extern const ASN1CType asn1_type_j2735BSMcoreData[];

typedef int j2735PartII_Id;

extern const ASN1CType asn1_type_j2735PartII_Id[];

typedef struct j2735PartIIcontent_1 {
  j2735PartII_Id partII_Id;
  ASN1OpenType partII_Value;
} j2735PartIIcontent_1;


extern const ASN1CType asn1_type_j2735PartIIcontent_1[];

typedef struct j2735BasicSafetyMessage_1 {
  j2735PartIIcontent_1 *tab;
  size_t count;
} j2735BasicSafetyMessage_1;

extern const ASN1CType asn1_type_j2735BasicSafetyMessage_1[];

typedef int j2735RegionId;

extern const ASN1CType asn1_type_j2735RegionId[];

typedef struct j2735RegionalExtension_1 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_1;


extern const ASN1CType asn1_type_j2735RegionalExtension_1[];

typedef struct j2735BasicSafetyMessage_2 {
  j2735RegionalExtension_1 *tab;
  size_t count;
} j2735BasicSafetyMessage_2;

extern const ASN1CType asn1_type_j2735BasicSafetyMessage_2[];

typedef struct j2735BasicSafetyMessage {
  j2735BSMcoreData coreData;
  BOOL partII_option;
  j2735BasicSafetyMessage_1 partII;
  BOOL regional_option;
  j2735BasicSafetyMessage_2 regional;
} j2735BasicSafetyMessage;


extern const ASN1CType asn1_type_j2735BasicSafetyMessage[];

typedef int j2735MinuteOfTheYear;

extern const ASN1CType asn1_type_j2735MinuteOfTheYear[];

typedef enum j2735LayerType {
  j2735LayerType_none,
  j2735LayerType_mixedContent,
  j2735LayerType_generalMapData,
  j2735LayerType_intersectionData,
  j2735LayerType_curveData,
  j2735LayerType_roadwaySectionData,
  j2735LayerType_parkingAreaData,
  j2735LayerType_sharedLaneData,
} j2735LayerType;

extern const ASN1CType asn1_type_j2735LayerType[];

typedef int j2735LayerID;

extern const ASN1CType asn1_type_j2735LayerID[];


typedef ASN1String j2735DescriptiveName;
extern const ASN1CType asn1_type_j2735DescriptiveName[];

typedef int j2735RoadRegulatorID;

extern const ASN1CType asn1_type_j2735RoadRegulatorID[];

typedef int j2735IntersectionID;

extern const ASN1CType asn1_type_j2735IntersectionID[];

typedef struct j2735IntersectionReferenceID {
  BOOL region_option;
  j2735RoadRegulatorID region;
  j2735IntersectionID id;
} j2735IntersectionReferenceID;


extern const ASN1CType asn1_type_j2735IntersectionReferenceID[];

typedef struct j2735RegionalExtension_49 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_49;


extern const ASN1CType asn1_type_j2735RegionalExtension_49[];

typedef struct j2735Position3D_1 {
  j2735RegionalExtension_49 *tab;
  size_t count;
} j2735Position3D_1;

extern const ASN1CType asn1_type_j2735Position3D_1[];

typedef struct j2735Position3D {
  j2735Latitude lat;
  j2735Longitude Long;
  BOOL elevation_option;
  j2735Elevation elevation;
  BOOL regional_option;
  j2735Position3D_1 regional;
} j2735Position3D;


extern const ASN1CType asn1_type_j2735Position3D[];

typedef int j2735LaneWidth;

extern const ASN1CType asn1_type_j2735LaneWidth[];

typedef enum j2735SpeedLimitType {
  j2735SpeedLimitType_unknown,
  j2735SpeedLimitType_maxSpeedInSchoolZone,
  j2735SpeedLimitType_maxSpeedInSchoolZoneWhenChildrenArePresent,
  j2735SpeedLimitType_maxSpeedInConstructionZone,
  j2735SpeedLimitType_vehicleMinSpeed,
  j2735SpeedLimitType_vehicleMaxSpeed,
  j2735SpeedLimitType_vehicleNightMaxSpeed,
  j2735SpeedLimitType_truckMinSpeed,
  j2735SpeedLimitType_truckMaxSpeed,
  j2735SpeedLimitType_truckNightMaxSpeed,
  j2735SpeedLimitType_vehiclesWithTrailersMinSpeed,
  j2735SpeedLimitType_vehiclesWithTrailersMaxSpeed,
  j2735SpeedLimitType_vehiclesWithTrailersNightMaxSpeed,
} j2735SpeedLimitType;

extern const ASN1CType asn1_type_j2735SpeedLimitType[];

typedef int j2735Velocity;

extern const ASN1CType asn1_type_j2735Velocity[];

typedef struct j2735RegulatorySpeedLimit {
  j2735SpeedLimitType type;
  j2735Velocity speed;
} j2735RegulatorySpeedLimit;


extern const ASN1CType asn1_type_j2735RegulatorySpeedLimit[];

typedef struct j2735SpeedLimitList {
  j2735RegulatorySpeedLimit *tab;
  size_t count;
} j2735SpeedLimitList;

extern const ASN1CType asn1_type_j2735SpeedLimitList[];

typedef int j2735LaneID;

extern const ASN1CType asn1_type_j2735LaneID[];

typedef int j2735ApproachID;

extern const ASN1CType asn1_type_j2735ApproachID[];

typedef ASN1BitString j2735LaneDirection;

extern const ASN1CType asn1_type_j2735LaneDirection[];

typedef ASN1BitString j2735LaneSharing;

extern const ASN1CType asn1_type_j2735LaneSharing[];

typedef ASN1BitString j2735LaneAttributes_Vehicle;

extern const ASN1CType asn1_type_j2735LaneAttributes_Vehicle[];

typedef ASN1BitString j2735LaneAttributes_Crosswalk;

extern const ASN1CType asn1_type_j2735LaneAttributes_Crosswalk[];

typedef ASN1BitString j2735LaneAttributes_Bike;

extern const ASN1CType asn1_type_j2735LaneAttributes_Bike[];

typedef ASN1BitString j2735LaneAttributes_Sidewalk;

extern const ASN1CType asn1_type_j2735LaneAttributes_Sidewalk[];

typedef ASN1BitString j2735LaneAttributes_Barrier;

extern const ASN1CType asn1_type_j2735LaneAttributes_Barrier[];

typedef ASN1BitString j2735LaneAttributes_Striping;

extern const ASN1CType asn1_type_j2735LaneAttributes_Striping[];

typedef ASN1BitString j2735LaneAttributes_TrackedVehicle;

extern const ASN1CType asn1_type_j2735LaneAttributes_TrackedVehicle[];

typedef ASN1BitString j2735LaneAttributes_Parking;

extern const ASN1CType asn1_type_j2735LaneAttributes_Parking[];

typedef enum {
  j2735LaneTypeAttributes_vehicle,
  j2735LaneTypeAttributes_crosswalk,
  j2735LaneTypeAttributes_bikeLane,
  j2735LaneTypeAttributes_sidewalk,
  j2735LaneTypeAttributes_median,
  j2735LaneTypeAttributes_striping,
  j2735LaneTypeAttributes_trackedVehicle,
  j2735LaneTypeAttributes_parking,
} j2735LaneTypeAttributes_choice;

typedef struct j2735LaneTypeAttributes {
  j2735LaneTypeAttributes_choice choice;
  union {
    j2735LaneAttributes_Vehicle vehicle;
    j2735LaneAttributes_Crosswalk crosswalk;
    j2735LaneAttributes_Bike bikeLane;
    j2735LaneAttributes_Sidewalk sidewalk;
    j2735LaneAttributes_Barrier median;
    j2735LaneAttributes_Striping striping;
    j2735LaneAttributes_TrackedVehicle trackedVehicle;
    j2735LaneAttributes_Parking parking;
  } u;
} j2735LaneTypeAttributes;

extern const ASN1CType asn1_type_j2735LaneTypeAttributes[];

typedef struct j2735RegionalExtension_41 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_41;


extern const ASN1CType asn1_type_j2735RegionalExtension_41[];

typedef struct j2735LaneAttributes {
  j2735LaneDirection directionalUse;
  j2735LaneSharing sharedWith;
  j2735LaneTypeAttributes laneType;
  BOOL regional_option;
  j2735RegionalExtension_41 regional;
} j2735LaneAttributes;


extern const ASN1CType asn1_type_j2735LaneAttributes[];

typedef ASN1BitString j2735AllowedManeuvers;

extern const ASN1CType asn1_type_j2735AllowedManeuvers[];

typedef int j2735Offset_B10;

extern const ASN1CType asn1_type_j2735Offset_B10[];

typedef struct j2735Node_XY_20b {
  j2735Offset_B10 x;
  j2735Offset_B10 y;
} j2735Node_XY_20b;


extern const ASN1CType asn1_type_j2735Node_XY_20b[];

typedef int j2735Offset_B11;

extern const ASN1CType asn1_type_j2735Offset_B11[];

typedef struct j2735Node_XY_22b {
  j2735Offset_B11 x;
  j2735Offset_B11 y;
} j2735Node_XY_22b;


extern const ASN1CType asn1_type_j2735Node_XY_22b[];

typedef int j2735Offset_B12;

extern const ASN1CType asn1_type_j2735Offset_B12[];

typedef struct j2735Node_XY_24b {
  j2735Offset_B12 x;
  j2735Offset_B12 y;
} j2735Node_XY_24b;


extern const ASN1CType asn1_type_j2735Node_XY_24b[];

typedef int j2735Offset_B13;

extern const ASN1CType asn1_type_j2735Offset_B13[];

typedef struct j2735Node_XY_26b {
  j2735Offset_B13 x;
  j2735Offset_B13 y;
} j2735Node_XY_26b;


extern const ASN1CType asn1_type_j2735Node_XY_26b[];

typedef int j2735Offset_B14;

extern const ASN1CType asn1_type_j2735Offset_B14[];

typedef struct j2735Node_XY_28b {
  j2735Offset_B14 x;
  j2735Offset_B14 y;
} j2735Node_XY_28b;


extern const ASN1CType asn1_type_j2735Node_XY_28b[];

typedef int j2735Offset_B16;

extern const ASN1CType asn1_type_j2735Offset_B16[];

typedef struct j2735Node_XY_32b {
  j2735Offset_B16 x;
  j2735Offset_B16 y;
} j2735Node_XY_32b;


extern const ASN1CType asn1_type_j2735Node_XY_32b[];

typedef struct j2735Node_LLmD_64b {
  j2735Longitude lon;
  j2735Latitude lat;
} j2735Node_LLmD_64b;


extern const ASN1CType asn1_type_j2735Node_LLmD_64b[];

typedef struct j2735RegionalExtension_48 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_48;


extern const ASN1CType asn1_type_j2735RegionalExtension_48[];

typedef enum {
  j2735NodeOffsetPointXY_node_XY1,
  j2735NodeOffsetPointXY_node_XY2,
  j2735NodeOffsetPointXY_node_XY3,
  j2735NodeOffsetPointXY_node_XY4,
  j2735NodeOffsetPointXY_node_XY5,
  j2735NodeOffsetPointXY_node_XY6,
  j2735NodeOffsetPointXY_node_LatLon,
  j2735NodeOffsetPointXY_regional,
} j2735NodeOffsetPointXY_choice;

typedef struct j2735NodeOffsetPointXY {
  j2735NodeOffsetPointXY_choice choice;
  union {
    j2735Node_XY_20b node_XY1;
    j2735Node_XY_22b node_XY2;
    j2735Node_XY_24b node_XY3;
    j2735Node_XY_26b node_XY4;
    j2735Node_XY_28b node_XY5;
    j2735Node_XY_32b node_XY6;
    j2735Node_LLmD_64b node_LatLon;
    j2735RegionalExtension_48 regional;
  } u;
} j2735NodeOffsetPointXY;

extern const ASN1CType asn1_type_j2735NodeOffsetPointXY[];

typedef enum j2735NodeAttributeXY {
  j2735NodeAttributeXY_reserved,
  j2735NodeAttributeXY_stopLine,
  j2735NodeAttributeXY_roundedCapStyleA,
  j2735NodeAttributeXY_roundedCapStyleB,
  j2735NodeAttributeXY_mergePoint,
  j2735NodeAttributeXY_divergePoint,
  j2735NodeAttributeXY_downstreamStopLine,
  j2735NodeAttributeXY_downstreamStartNode,
  j2735NodeAttributeXY_closedToTraffic,
  j2735NodeAttributeXY_safeIsland,
  j2735NodeAttributeXY_curbPresentAtStepOff,
  j2735NodeAttributeXY_hydrantPresent,
} j2735NodeAttributeXY;

extern const ASN1CType asn1_type_j2735NodeAttributeXY[];

typedef struct j2735NodeAttributeXYList {
  j2735NodeAttributeXY *tab;
  size_t count;
} j2735NodeAttributeXYList;

extern const ASN1CType asn1_type_j2735NodeAttributeXYList[];

typedef enum j2735SegmentAttributeXY {
  j2735SegmentAttributeXY_reserved,
  j2735SegmentAttributeXY_doNotBlock,
  j2735SegmentAttributeXY_whiteLine,
  j2735SegmentAttributeXY_mergingLaneLeft,
  j2735SegmentAttributeXY_mergingLaneRight,
  j2735SegmentAttributeXY_curbOnLeft,
  j2735SegmentAttributeXY_curbOnRight,
  j2735SegmentAttributeXY_loadingzoneOnLeft,
  j2735SegmentAttributeXY_loadingzoneOnRight,
  j2735SegmentAttributeXY_turnOutPointOnLeft,
  j2735SegmentAttributeXY_turnOutPointOnRight,
  j2735SegmentAttributeXY_adjacentParkingOnLeft,
  j2735SegmentAttributeXY_adjacentParkingOnRight,
  j2735SegmentAttributeXY_adjacentBikeLaneOnLeft,
  j2735SegmentAttributeXY_adjacentBikeLaneOnRight,
  j2735SegmentAttributeXY_sharedBikeLane,
  j2735SegmentAttributeXY_bikeBoxInFront,
  j2735SegmentAttributeXY_transitStopOnLeft,
  j2735SegmentAttributeXY_transitStopOnRight,
  j2735SegmentAttributeXY_transitStopInLane,
  j2735SegmentAttributeXY_sharedWithTrackedVehicle,
  j2735SegmentAttributeXY_safeIsland,
  j2735SegmentAttributeXY_lowCurbsPresent,
  j2735SegmentAttributeXY_rumbleStripPresent,
  j2735SegmentAttributeXY_audibleSignalingPresent,
  j2735SegmentAttributeXY_adaptiveTimingPresent,
  j2735SegmentAttributeXY_rfSignalRequestPresent,
  j2735SegmentAttributeXY_partialCurbIntrusion,
  j2735SegmentAttributeXY_taperToLeft,
  j2735SegmentAttributeXY_taperToRight,
  j2735SegmentAttributeXY_taperToCenterLine,
  j2735SegmentAttributeXY_parallelParking,
  j2735SegmentAttributeXY_headInParking,
  j2735SegmentAttributeXY_freeParking,
  j2735SegmentAttributeXY_timeRestrictionsOnParking,
  j2735SegmentAttributeXY_costToPark,
  j2735SegmentAttributeXY_midBlockCurbPresent,
  j2735SegmentAttributeXY_unEvenPavementPresent,
} j2735SegmentAttributeXY;

extern const ASN1CType asn1_type_j2735SegmentAttributeXY[];

typedef struct j2735SegmentAttributeXYList {
  j2735SegmentAttributeXY *tab;
  size_t count;
} j2735SegmentAttributeXYList;

extern const ASN1CType asn1_type_j2735SegmentAttributeXYList[];

typedef int j2735DeltaAngle;

extern const ASN1CType asn1_type_j2735DeltaAngle[];

typedef int j2735RoadwayCrownAngle;

extern const ASN1CType asn1_type_j2735RoadwayCrownAngle[];

typedef int j2735MergeDivergeNodeAngle;

extern const ASN1CType asn1_type_j2735MergeDivergeNodeAngle[];

typedef struct j2735RegionalExtension_42 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_42;


extern const ASN1CType asn1_type_j2735RegionalExtension_42[];

typedef struct j2735LaneDataAttribute_1 {
  j2735RegionalExtension_42 *tab;
  size_t count;
} j2735LaneDataAttribute_1;

extern const ASN1CType asn1_type_j2735LaneDataAttribute_1[];

typedef enum {
  j2735LaneDataAttribute_pathEndPointAngle,
  j2735LaneDataAttribute_laneCrownPointCenter,
  j2735LaneDataAttribute_laneCrownPointLeft,
  j2735LaneDataAttribute_laneCrownPointRight,
  j2735LaneDataAttribute_laneAngle,
  j2735LaneDataAttribute_speedLimits,
  j2735LaneDataAttribute_regional,
} j2735LaneDataAttribute_choice;

typedef struct j2735LaneDataAttribute {
  j2735LaneDataAttribute_choice choice;
  union {
    j2735DeltaAngle pathEndPointAngle;
    j2735RoadwayCrownAngle laneCrownPointCenter;
    j2735RoadwayCrownAngle laneCrownPointLeft;
    j2735RoadwayCrownAngle laneCrownPointRight;
    j2735MergeDivergeNodeAngle laneAngle;
    j2735SpeedLimitList speedLimits;
    j2735LaneDataAttribute_1 regional;
  } u;
} j2735LaneDataAttribute;

extern const ASN1CType asn1_type_j2735LaneDataAttribute[];

typedef struct j2735LaneDataAttributeList {
  j2735LaneDataAttribute *tab;
  size_t count;
} j2735LaneDataAttributeList;

extern const ASN1CType asn1_type_j2735LaneDataAttributeList[];

typedef struct j2735RegionalExtension_46 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_46;


extern const ASN1CType asn1_type_j2735RegionalExtension_46[];

typedef struct j2735NodeAttributeSetXY_1 {
  j2735RegionalExtension_46 *tab;
  size_t count;
} j2735NodeAttributeSetXY_1;

extern const ASN1CType asn1_type_j2735NodeAttributeSetXY_1[];

typedef struct j2735NodeAttributeSetXY {
  BOOL localNode_option;
  j2735NodeAttributeXYList localNode;
  BOOL disabled_option;
  j2735SegmentAttributeXYList disabled;
  BOOL enabled_option;
  j2735SegmentAttributeXYList enabled;
  BOOL data_option;
  j2735LaneDataAttributeList data;
  BOOL dWidth_option;
  j2735Offset_B10 dWidth;
  BOOL dElevation_option;
  j2735Offset_B10 dElevation;
  BOOL regional_option;
  j2735NodeAttributeSetXY_1 regional;
} j2735NodeAttributeSetXY;


extern const ASN1CType asn1_type_j2735NodeAttributeSetXY[];

typedef struct j2735NodeXY {
  j2735NodeOffsetPointXY delta;
  BOOL attributes_option;
  j2735NodeAttributeSetXY attributes;
} j2735NodeXY;


extern const ASN1CType asn1_type_j2735NodeXY[];

typedef struct j2735NodeSetXY {
  j2735NodeXY *tab;
  size_t count;
} j2735NodeSetXY;

extern const ASN1CType asn1_type_j2735NodeSetXY[];

typedef int j2735DrivenLineOffsetSm;

extern const ASN1CType asn1_type_j2735DrivenLineOffsetSm[];

typedef int j2735DrivenLineOffsetLg;

extern const ASN1CType asn1_type_j2735DrivenLineOffsetLg[];

typedef enum {
  j2735ComputedLane_1_small,
  j2735ComputedLane_1_large,
} j2735ComputedLane_1_choice;

typedef struct j2735ComputedLane_1 {
  j2735ComputedLane_1_choice choice;
  union {
    j2735DrivenLineOffsetSm small;
    j2735DrivenLineOffsetLg large;
  } u;
} j2735ComputedLane_1;

extern const ASN1CType asn1_type_j2735ComputedLane_1[];

typedef enum {
  j2735ComputedLane_2_small,
  j2735ComputedLane_2_large,
} j2735ComputedLane_2_choice;

typedef struct j2735ComputedLane_2 {
  j2735ComputedLane_2_choice choice;
  union {
    j2735DrivenLineOffsetSm small;
    j2735DrivenLineOffsetLg large;
  } u;
} j2735ComputedLane_2;

extern const ASN1CType asn1_type_j2735ComputedLane_2[];

typedef int j2735Angle;

extern const ASN1CType asn1_type_j2735Angle[];

typedef int j2735Scale_B12;

extern const ASN1CType asn1_type_j2735Scale_B12[];

typedef struct j2735RegionalExtension_33 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_33;


extern const ASN1CType asn1_type_j2735RegionalExtension_33[];

typedef struct j2735ComputedLane_3 {
  j2735RegionalExtension_33 *tab;
  size_t count;
} j2735ComputedLane_3;

extern const ASN1CType asn1_type_j2735ComputedLane_3[];

typedef struct j2735ComputedLane {
  j2735LaneID referenceLaneId;
  j2735ComputedLane_1 offsetXaxis;
  j2735ComputedLane_2 offsetYaxis;
  BOOL rotateXY_option;
  j2735Angle rotateXY;
  BOOL scaleXaxis_option;
  j2735Scale_B12 scaleXaxis;
  BOOL scaleYaxis_option;
  j2735Scale_B12 scaleYaxis;
  BOOL regional_option;
  j2735ComputedLane_3 regional;
} j2735ComputedLane;


extern const ASN1CType asn1_type_j2735ComputedLane[];

typedef enum {
  j2735NodeListXY_nodes,
  j2735NodeListXY_computed,
} j2735NodeListXY_choice;

typedef struct j2735NodeListXY {
  j2735NodeListXY_choice choice;
  union {
    j2735NodeSetXY nodes;
    j2735ComputedLane computed;
  } u;
} j2735NodeListXY;

extern const ASN1CType asn1_type_j2735NodeListXY[];

typedef struct j2735ConnectingLane {
  j2735LaneID lane;
  BOOL maneuver_option;
  j2735AllowedManeuvers maneuver;
} j2735ConnectingLane;


extern const ASN1CType asn1_type_j2735ConnectingLane[];

typedef int j2735SignalGroupID;

extern const ASN1CType asn1_type_j2735SignalGroupID[];

typedef int j2735RestrictionClassID;

extern const ASN1CType asn1_type_j2735RestrictionClassID[];

typedef int j2735LaneConnectionID;

extern const ASN1CType asn1_type_j2735LaneConnectionID[];

typedef struct j2735Connection {
  j2735ConnectingLane connectingLane;
  BOOL remoteIntersection_option;
  j2735IntersectionReferenceID remoteIntersection;
  BOOL signalGroup_option;
  j2735SignalGroupID signalGroup;
  BOOL userClass_option;
  j2735RestrictionClassID userClass;
  BOOL connectionID_option;
  j2735LaneConnectionID connectionID;
} j2735Connection;


extern const ASN1CType asn1_type_j2735Connection[];

typedef struct j2735ConnectsToList {
  j2735Connection *tab;
  size_t count;
} j2735ConnectsToList;

extern const ASN1CType asn1_type_j2735ConnectsToList[];

typedef struct j2735OverlayLaneList {
  j2735LaneID *tab;
  size_t count;
} j2735OverlayLaneList;

extern const ASN1CType asn1_type_j2735OverlayLaneList[];

typedef struct j2735RegionalExtension_36 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_36;


extern const ASN1CType asn1_type_j2735RegionalExtension_36[];

typedef struct j2735GenericLane_1 {
  j2735RegionalExtension_36 *tab;
  size_t count;
} j2735GenericLane_1;

extern const ASN1CType asn1_type_j2735GenericLane_1[];

typedef struct j2735GenericLane {
  j2735LaneID laneID;
  BOOL name_option;
  j2735DescriptiveName name;
  BOOL ingressApproach_option;
  j2735ApproachID ingressApproach;
  BOOL egressApproach_option;
  j2735ApproachID egressApproach;
  j2735LaneAttributes laneAttributes;
  BOOL maneuvers_option;
  j2735AllowedManeuvers maneuvers;
  j2735NodeListXY nodeList;
  BOOL connectsTo_option;
  j2735ConnectsToList connectsTo;
  BOOL overlays_option;
  j2735OverlayLaneList overlays;
  BOOL regional_option;
  j2735GenericLane_1 regional;
} j2735GenericLane;


extern const ASN1CType asn1_type_j2735GenericLane[];

typedef struct j2735LaneList {
  j2735GenericLane *tab;
  size_t count;
} j2735LaneList;

extern const ASN1CType asn1_type_j2735LaneList[];

typedef struct j2735RegionalExtension_50 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_50;


extern const ASN1CType asn1_type_j2735RegionalExtension_50[];

typedef struct j2735SignalControlZone {
  j2735RegionalExtension_50 zone;
} j2735SignalControlZone;


extern const ASN1CType asn1_type_j2735SignalControlZone[];

typedef struct j2735PreemptPriorityList {
  j2735SignalControlZone *tab;
  size_t count;
} j2735PreemptPriorityList;

extern const ASN1CType asn1_type_j2735PreemptPriorityList[];

typedef struct j2735RegionalExtension_39 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_39;


extern const ASN1CType asn1_type_j2735RegionalExtension_39[];

typedef struct j2735IntersectionGeometry_1 {
  j2735RegionalExtension_39 *tab;
  size_t count;
} j2735IntersectionGeometry_1;

extern const ASN1CType asn1_type_j2735IntersectionGeometry_1[];

typedef struct j2735IntersectionGeometry {
  BOOL name_option;
  j2735DescriptiveName name;
  j2735IntersectionReferenceID id;
  j2735MsgCount revision;
  j2735Position3D refPoint;
  BOOL laneWidth_option;
  j2735LaneWidth laneWidth;
  BOOL speedLimits_option;
  j2735SpeedLimitList speedLimits;
  j2735LaneList laneSet;
  BOOL preemptPriorityData_option;
  j2735PreemptPriorityList preemptPriorityData;
  BOOL regional_option;
  j2735IntersectionGeometry_1 regional;
} j2735IntersectionGeometry;


extern const ASN1CType asn1_type_j2735IntersectionGeometry[];

typedef struct j2735IntersectionGeometryList {
  j2735IntersectionGeometry *tab;
  size_t count;
} j2735IntersectionGeometryList;

extern const ASN1CType asn1_type_j2735IntersectionGeometryList[];

typedef int j2735RoadSegmentID;

extern const ASN1CType asn1_type_j2735RoadSegmentID[];

typedef struct j2735RoadSegmentReferenceID {
  BOOL region_option;
  j2735RoadRegulatorID region;
  j2735RoadSegmentID id;
} j2735RoadSegmentReferenceID;


extern const ASN1CType asn1_type_j2735RoadSegmentReferenceID[];

typedef struct j2735RoadLaneSetList {
  j2735GenericLane *tab;
  size_t count;
} j2735RoadLaneSetList;

extern const ASN1CType asn1_type_j2735RoadLaneSetList[];

typedef struct j2735RegionalExtension_54 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_54;


extern const ASN1CType asn1_type_j2735RegionalExtension_54[];

typedef struct j2735RoadSegment_1 {
  j2735RegionalExtension_54 *tab;
  size_t count;
} j2735RoadSegment_1;

extern const ASN1CType asn1_type_j2735RoadSegment_1[];

typedef struct j2735RoadSegment {
  BOOL name_option;
  j2735DescriptiveName name;
  j2735RoadSegmentReferenceID id;
  j2735MsgCount revision;
  j2735Position3D refPoint;
  BOOL laneWidth_option;
  j2735LaneWidth laneWidth;
  BOOL speedLimits_option;
  j2735SpeedLimitList speedLimits;
  j2735RoadLaneSetList roadLaneSet;
  BOOL regional_option;
  j2735RoadSegment_1 regional;
} j2735RoadSegment;


extern const ASN1CType asn1_type_j2735RoadSegment[];

typedef struct j2735RoadSegmentList {
  j2735RoadSegment *tab;
  size_t count;
} j2735RoadSegmentList;

extern const ASN1CType asn1_type_j2735RoadSegmentList[];

typedef struct j2735DataParameters {
  BOOL processMethod_option;
  ASN1String processMethod;
  BOOL processAgency_option;
  ASN1String processAgency;
  BOOL lastCheckedDate_option;
  ASN1String lastCheckedDate;
  BOOL geoidUsed_option;
  ASN1String geoidUsed;
} j2735DataParameters;


extern const ASN1CType asn1_type_j2735DataParameters[];

typedef enum j2735RestrictionAppliesTo {
  j2735RestrictionAppliesTo_none,
  j2735RestrictionAppliesTo_equippedTransit,
  j2735RestrictionAppliesTo_equippedTaxis,
  j2735RestrictionAppliesTo_equippedOther,
  j2735RestrictionAppliesTo_emissionCompliant,
  j2735RestrictionAppliesTo_equippedBicycle,
  j2735RestrictionAppliesTo_weightCompliant,
  j2735RestrictionAppliesTo_heightCompliant,
  j2735RestrictionAppliesTo_pedestrians,
  j2735RestrictionAppliesTo_slowMovingPersons,
  j2735RestrictionAppliesTo_wheelchairUsers,
  j2735RestrictionAppliesTo_visualDisabilities,
  j2735RestrictionAppliesTo_audioDisabilities,
  j2735RestrictionAppliesTo_otherUnknownDisabilities,
} j2735RestrictionAppliesTo;

extern const ASN1CType asn1_type_j2735RestrictionAppliesTo[];

typedef struct j2735RegionalExtension_53 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_53;


extern const ASN1CType asn1_type_j2735RegionalExtension_53[];

typedef struct j2735RestrictionUserType_1 {
  j2735RegionalExtension_53 *tab;
  size_t count;
} j2735RestrictionUserType_1;

extern const ASN1CType asn1_type_j2735RestrictionUserType_1[];

typedef enum {
  j2735RestrictionUserType_basicType,
  j2735RestrictionUserType_regional,
} j2735RestrictionUserType_choice;

typedef struct j2735RestrictionUserType {
  j2735RestrictionUserType_choice choice;
  union {
    j2735RestrictionAppliesTo basicType;
    j2735RestrictionUserType_1 regional;
  } u;
} j2735RestrictionUserType;

extern const ASN1CType asn1_type_j2735RestrictionUserType[];

typedef struct j2735RestrictionUserTypeList {
  j2735RestrictionUserType *tab;
  size_t count;
} j2735RestrictionUserTypeList;

extern const ASN1CType asn1_type_j2735RestrictionUserTypeList[];

typedef struct j2735RestrictionClassAssignment {
  j2735RestrictionClassID id;
  j2735RestrictionUserTypeList users;
} j2735RestrictionClassAssignment;


extern const ASN1CType asn1_type_j2735RestrictionClassAssignment[];

typedef struct j2735RestrictionClassList {
  j2735RestrictionClassAssignment *tab;
  size_t count;
} j2735RestrictionClassList;

extern const ASN1CType asn1_type_j2735RestrictionClassList[];

typedef struct j2735RegionalExtension_5 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_5;


extern const ASN1CType asn1_type_j2735RegionalExtension_5[];

typedef struct j2735MapData_1 {
  j2735RegionalExtension_5 *tab;
  size_t count;
} j2735MapData_1;

extern const ASN1CType asn1_type_j2735MapData_1[];

typedef struct j2735MapData {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  j2735MsgCount msgIssueRevision;
  BOOL layerType_option;
  j2735LayerType layerType;
  BOOL layerID_option;
  j2735LayerID layerID;
  BOOL intersections_option;
  j2735IntersectionGeometryList intersections;
  BOOL roadSegments_option;
  j2735RoadSegmentList roadSegments;
  BOOL dataParameters_option;
  j2735DataParameters dataParameters;
  BOOL restrictionList_option;
  j2735RestrictionClassList restrictionList;
  BOOL regional_option;
  j2735MapData_1 regional;
} j2735MapData;


extern const ASN1CType asn1_type_j2735MapData[];

typedef ASN1BitString j2735IntersectionStatusObject;

extern const ASN1CType asn1_type_j2735IntersectionStatusObject[];

typedef struct j2735EnabledLaneList {
  j2735LaneID *tab;
  size_t count;
} j2735EnabledLaneList;

extern const ASN1CType asn1_type_j2735EnabledLaneList[];

typedef enum j2735MovementPhaseState {
  j2735MovementPhaseState_unavailable,
  j2735MovementPhaseState_dark,
  j2735MovementPhaseState_stop_Then_Proceed,
  j2735MovementPhaseState_stop_And_Remain,
  j2735MovementPhaseState_pre_Movement,
  j2735MovementPhaseState_permissive_Movement_Allowed,
  j2735MovementPhaseState_protected_Movement_Allowed,
  j2735MovementPhaseState_permissive_clearance,
  j2735MovementPhaseState_protected_clearance,
  j2735MovementPhaseState_caution_Conflicting_Traffic,
} j2735MovementPhaseState;

extern const ASN1CType asn1_type_j2735MovementPhaseState[];

typedef int j2735TimeMark;

extern const ASN1CType asn1_type_j2735TimeMark[];

typedef int j2735TimeIntervalConfidence;

extern const ASN1CType asn1_type_j2735TimeIntervalConfidence[];

typedef struct j2735TimeChangeDetails {
  BOOL startTime_option;
  j2735TimeMark startTime;
  j2735TimeMark minEndTime;
  BOOL maxEndTime_option;
  j2735TimeMark maxEndTime;
  BOOL likelyTime_option;
  j2735TimeMark likelyTime;
  BOOL confidence_option;
  j2735TimeIntervalConfidence confidence;
  BOOL nextTime_option;
  j2735TimeMark nextTime;
} j2735TimeChangeDetails;


extern const ASN1CType asn1_type_j2735TimeChangeDetails[];

typedef enum j2735AdvisorySpeedType {
  j2735AdvisorySpeedType_none,
  j2735AdvisorySpeedType_greenwave,
  j2735AdvisorySpeedType_ecoDrive,
  j2735AdvisorySpeedType_transit,
} j2735AdvisorySpeedType;

extern const ASN1CType asn1_type_j2735AdvisorySpeedType[];

typedef int j2735SpeedAdvice;

extern const ASN1CType asn1_type_j2735SpeedAdvice[];

typedef enum j2735SpeedConfidence {
  j2735SpeedConfidence_unavailable,
  j2735SpeedConfidence_prec100ms,
  j2735SpeedConfidence_prec10ms,
  j2735SpeedConfidence_prec5ms,
  j2735SpeedConfidence_prec1ms,
  j2735SpeedConfidence_prec0_1ms,
  j2735SpeedConfidence_prec0_05ms,
  j2735SpeedConfidence_prec0_01ms,
} j2735SpeedConfidence;

extern const ASN1CType asn1_type_j2735SpeedConfidence[];

typedef int j2735ZoneLength;

extern const ASN1CType asn1_type_j2735ZoneLength[];

typedef struct j2735RegionalExtension_32 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_32;


extern const ASN1CType asn1_type_j2735RegionalExtension_32[];

typedef struct j2735AdvisorySpeed_1 {
  j2735RegionalExtension_32 *tab;
  size_t count;
} j2735AdvisorySpeed_1;

extern const ASN1CType asn1_type_j2735AdvisorySpeed_1[];

typedef struct j2735AdvisorySpeed {
  j2735AdvisorySpeedType type;
  BOOL speed_option;
  j2735SpeedAdvice speed;
  BOOL confidence_option;
  j2735SpeedConfidence confidence;
  BOOL distance_option;
  j2735ZoneLength distance;
  BOOL Class_option;
  j2735RestrictionClassID Class;
  BOOL regional_option;
  j2735AdvisorySpeed_1 regional;
} j2735AdvisorySpeed;


extern const ASN1CType asn1_type_j2735AdvisorySpeed[];

typedef struct j2735AdvisorySpeedList {
  j2735AdvisorySpeed *tab;
  size_t count;
} j2735AdvisorySpeedList;

extern const ASN1CType asn1_type_j2735AdvisorySpeedList[];

typedef struct j2735RegionalExtension_43 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_43;


extern const ASN1CType asn1_type_j2735RegionalExtension_43[];

typedef struct j2735MovementEvent_1 {
  j2735RegionalExtension_43 *tab;
  size_t count;
} j2735MovementEvent_1;

extern const ASN1CType asn1_type_j2735MovementEvent_1[];

typedef struct j2735MovementEvent {
  j2735MovementPhaseState eventState;
  BOOL timing_option;
  j2735TimeChangeDetails timing;
  BOOL speeds_option;
  j2735AdvisorySpeedList speeds;
  BOOL regional_option;
  j2735MovementEvent_1 regional;
} j2735MovementEvent;


extern const ASN1CType asn1_type_j2735MovementEvent[];

typedef struct j2735MovementEventList {
  j2735MovementEvent *tab;
  size_t count;
} j2735MovementEventList;

extern const ASN1CType asn1_type_j2735MovementEventList[];

typedef BOOL j2735WaitOnStopline;

extern const ASN1CType asn1_type_j2735WaitOnStopline[];

typedef BOOL j2735PedestrianBicycleDetect;

extern const ASN1CType asn1_type_j2735PedestrianBicycleDetect[];

typedef struct j2735RegionalExtension_34 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_34;


extern const ASN1CType asn1_type_j2735RegionalExtension_34[];

typedef struct j2735ConnectionManeuverAssist_1 {
  j2735RegionalExtension_34 *tab;
  size_t count;
} j2735ConnectionManeuverAssist_1;

extern const ASN1CType asn1_type_j2735ConnectionManeuverAssist_1[];

typedef struct j2735ConnectionManeuverAssist {
  j2735LaneConnectionID connectionID;
  BOOL queueLength_option;
  j2735ZoneLength queueLength;
  BOOL availableStorageLength_option;
  j2735ZoneLength availableStorageLength;
  BOOL waitOnStop_option;
  j2735WaitOnStopline waitOnStop;
  BOOL pedBicycleDetect_option;
  j2735PedestrianBicycleDetect pedBicycleDetect;
  BOOL regional_option;
  j2735ConnectionManeuverAssist_1 regional;
} j2735ConnectionManeuverAssist;


extern const ASN1CType asn1_type_j2735ConnectionManeuverAssist[];

typedef struct j2735ManeuverAssistList {
  j2735ConnectionManeuverAssist *tab;
  size_t count;
} j2735ManeuverAssistList;

extern const ASN1CType asn1_type_j2735ManeuverAssistList[];

typedef struct j2735RegionalExtension_44 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_44;


extern const ASN1CType asn1_type_j2735RegionalExtension_44[];

typedef struct j2735MovementState_1 {
  j2735RegionalExtension_44 *tab;
  size_t count;
} j2735MovementState_1;

extern const ASN1CType asn1_type_j2735MovementState_1[];

typedef struct j2735MovementState {
  BOOL movementName_option;
  j2735DescriptiveName movementName;
  j2735SignalGroupID signalGroup;
  j2735MovementEventList state_time_speed;
  BOOL maneuverAssistList_option;
  j2735ManeuverAssistList maneuverAssistList;
  BOOL regional_option;
  j2735MovementState_1 regional;
} j2735MovementState;


extern const ASN1CType asn1_type_j2735MovementState[];

typedef struct j2735MovementList {
  j2735MovementState *tab;
  size_t count;
} j2735MovementList;

extern const ASN1CType asn1_type_j2735MovementList[];

typedef struct j2735RegionalExtension_40 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_40;


extern const ASN1CType asn1_type_j2735RegionalExtension_40[];

typedef struct j2735IntersectionState_1 {
  j2735RegionalExtension_40 *tab;
  size_t count;
} j2735IntersectionState_1;

extern const ASN1CType asn1_type_j2735IntersectionState_1[];

typedef struct j2735IntersectionState {
  BOOL name_option;
  j2735DescriptiveName name;
  j2735IntersectionReferenceID id;
  j2735MsgCount revision;
  j2735IntersectionStatusObject status;
  BOOL moy_option;
  j2735MinuteOfTheYear moy;
  BOOL timeStamp_option;
  j2735DSecond timeStamp;
  BOOL enabledLanes_option;
  j2735EnabledLaneList enabledLanes;
  j2735MovementList states;
  BOOL maneuverAssistList_option;
  j2735ManeuverAssistList maneuverAssistList;
  BOOL regional_option;
  j2735IntersectionState_1 regional;
} j2735IntersectionState;


extern const ASN1CType asn1_type_j2735IntersectionState[];

typedef struct j2735IntersectionStateList {
  j2735IntersectionState *tab;
  size_t count;
} j2735IntersectionStateList;

extern const ASN1CType asn1_type_j2735IntersectionStateList[];

typedef struct j2735RegionalExtension_12 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_12;


extern const ASN1CType asn1_type_j2735RegionalExtension_12[];

typedef struct j2735SPAT_1 {
  j2735RegionalExtension_12 *tab;
  size_t count;
} j2735SPAT_1;

extern const ASN1CType asn1_type_j2735SPAT_1[];

typedef struct j2735SPAT {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL name_option;
  j2735DescriptiveName name;
  j2735IntersectionStateList intersections;
  BOOL regional_option;
  j2735SPAT_1 regional;
} j2735SPAT;


extern const ASN1CType asn1_type_j2735SPAT[];

typedef enum j2735RequestedItem {
  j2735RequestedItem_reserved,
  j2735RequestedItem_itemA,
  j2735RequestedItem_itemB,
  j2735RequestedItem_itemC,
  j2735RequestedItem_itemD,
  j2735RequestedItem_itemE,
  j2735RequestedItem_itemF,
  j2735RequestedItem_itemG,
  j2735RequestedItem_itemI,
  j2735RequestedItem_itemJ,
  j2735RequestedItem_itemK,
  j2735RequestedItem_itemL,
  j2735RequestedItem_itemM,
  j2735RequestedItem_itemN,
  j2735RequestedItem_itemO,
  j2735RequestedItem_itemP,
  j2735RequestedItem_itemQ,
} j2735RequestedItem;

extern const ASN1CType asn1_type_j2735RequestedItem[];

typedef struct j2735RequestedItemList {
  j2735RequestedItem *tab;
  size_t count;
} j2735RequestedItemList;

extern const ASN1CType asn1_type_j2735RequestedItemList[];

typedef struct j2735RegionalExtension_2 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_2;


extern const ASN1CType asn1_type_j2735RegionalExtension_2[];

typedef struct j2735CommonSafetyRequest_1 {
  j2735RegionalExtension_2 *tab;
  size_t count;
} j2735CommonSafetyRequest_1;

extern const ASN1CType asn1_type_j2735CommonSafetyRequest_1[];

typedef struct j2735CommonSafetyRequest {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL msgCnt_option;
  j2735MsgCount msgCnt;
  BOOL id_option;
  j2735TemporaryID id;
  j2735RequestedItemList requests;
  BOOL regional_option;
  j2735CommonSafetyRequest_1 regional;
} j2735CommonSafetyRequest;


extern const ASN1CType asn1_type_j2735CommonSafetyRequest[];

typedef int j2735ITIScodes;

extern const ASN1CType asn1_type_j2735ITIScodes[];

typedef struct j2735RoadSideAlert_1 {
  j2735ITIScodes *tab;
  size_t count;
} j2735RoadSideAlert_1;

extern const ASN1CType asn1_type_j2735RoadSideAlert_1[];

typedef ASN1String j2735Priority;

extern const ASN1CType asn1_type_j2735Priority[];

typedef ASN1BitString j2735HeadingSlice;

extern const ASN1CType asn1_type_j2735HeadingSlice[];

typedef enum j2735Extent {
  j2735Extent_useInstantlyOnly,
  j2735Extent_useFor3meters,
  j2735Extent_useFor10meters,
  j2735Extent_useFor50meters,
  j2735Extent_useFor100meters,
  j2735Extent_useFor500meters,
  j2735Extent_useFor1000meters,
  j2735Extent_useFor5000meters,
  j2735Extent_useFor10000meters,
  j2735Extent_useFor50000meters,
  j2735Extent_useFor100000meters,
  j2735Extent_useFor500000meters,
  j2735Extent_useFor1000000meters,
  j2735Extent_useFor5000000meters,
  j2735Extent_useFor10000000meters,
  j2735Extent_forever,
} j2735Extent;

extern const ASN1CType asn1_type_j2735Extent[];

typedef int j2735DYear;

extern const ASN1CType asn1_type_j2735DYear[];

typedef int j2735DMonth;

extern const ASN1CType asn1_type_j2735DMonth[];

typedef int j2735DDay;

extern const ASN1CType asn1_type_j2735DDay[];

typedef int j2735DHour;

extern const ASN1CType asn1_type_j2735DHour[];

typedef int j2735DMinute;

extern const ASN1CType asn1_type_j2735DMinute[];

typedef int j2735DOffset;

extern const ASN1CType asn1_type_j2735DOffset[];

typedef struct j2735DDateTime {
  BOOL year_option;
  j2735DYear year;
  BOOL month_option;
  j2735DMonth month;
  BOOL day_option;
  j2735DDay day;
  BOOL hour_option;
  j2735DHour hour;
  BOOL minute_option;
  j2735DMinute minute;
  BOOL second_option;
  j2735DSecond second;
  BOOL offset_option;
  j2735DOffset offset;
} j2735DDateTime;


extern const ASN1CType asn1_type_j2735DDateTime[];

typedef struct j2735TransmissionAndSpeed {
  j2735TransmissionState transmisson;
  j2735Velocity speed;
} j2735TransmissionAndSpeed;


extern const ASN1CType asn1_type_j2735TransmissionAndSpeed[];

typedef enum j2735TimeConfidence {
  j2735TimeConfidence_unavailable,
  j2735TimeConfidence_time_100_000,
  j2735TimeConfidence_time_050_000,
  j2735TimeConfidence_time_020_000,
  j2735TimeConfidence_time_010_000,
  j2735TimeConfidence_time_002_000,
  j2735TimeConfidence_time_001_000,
  j2735TimeConfidence_time_000_500,
  j2735TimeConfidence_time_000_200,
  j2735TimeConfidence_time_000_100,
  j2735TimeConfidence_time_000_050,
  j2735TimeConfidence_time_000_020,
  j2735TimeConfidence_time_000_010,
  j2735TimeConfidence_time_000_005,
  j2735TimeConfidence_time_000_002,
  j2735TimeConfidence_time_000_001,
  j2735TimeConfidence_time_000_000_5,
  j2735TimeConfidence_time_000_000_2,
  j2735TimeConfidence_time_000_000_1,
  j2735TimeConfidence_time_000_000_05,
  j2735TimeConfidence_time_000_000_02,
  j2735TimeConfidence_time_000_000_01,
  j2735TimeConfidence_time_000_000_005,
  j2735TimeConfidence_time_000_000_002,
  j2735TimeConfidence_time_000_000_001,
  j2735TimeConfidence_time_000_000_000_5,
  j2735TimeConfidence_time_000_000_000_2,
  j2735TimeConfidence_time_000_000_000_1,
  j2735TimeConfidence_time_000_000_000_05,
  j2735TimeConfidence_time_000_000_000_02,
  j2735TimeConfidence_time_000_000_000_01,
  j2735TimeConfidence_time_000_000_000_005,
  j2735TimeConfidence_time_000_000_000_002,
  j2735TimeConfidence_time_000_000_000_001,
  j2735TimeConfidence_time_000_000_000_000_5,
  j2735TimeConfidence_time_000_000_000_000_2,
  j2735TimeConfidence_time_000_000_000_000_1,
  j2735TimeConfidence_time_000_000_000_000_05,
  j2735TimeConfidence_time_000_000_000_000_02,
  j2735TimeConfidence_time_000_000_000_000_01,
} j2735TimeConfidence;

extern const ASN1CType asn1_type_j2735TimeConfidence[];

typedef enum j2735PositionConfidence {
  j2735PositionConfidence_unavailable,
  j2735PositionConfidence_a500m,
  j2735PositionConfidence_a200m,
  j2735PositionConfidence_a100m,
  j2735PositionConfidence_a50m,
  j2735PositionConfidence_a20m,
  j2735PositionConfidence_a10m,
  j2735PositionConfidence_a5m,
  j2735PositionConfidence_a2m,
  j2735PositionConfidence_a1m,
  j2735PositionConfidence_a50cm,
  j2735PositionConfidence_a20cm,
  j2735PositionConfidence_a10cm,
  j2735PositionConfidence_a5cm,
  j2735PositionConfidence_a2cm,
  j2735PositionConfidence_a1cm,
} j2735PositionConfidence;

extern const ASN1CType asn1_type_j2735PositionConfidence[];

typedef enum j2735ElevationConfidence {
  j2735ElevationConfidence_unavailable,
  j2735ElevationConfidence_elev_500_00,
  j2735ElevationConfidence_elev_200_00,
  j2735ElevationConfidence_elev_100_00,
  j2735ElevationConfidence_elev_050_00,
  j2735ElevationConfidence_elev_020_00,
  j2735ElevationConfidence_elev_010_00,
  j2735ElevationConfidence_elev_005_00,
  j2735ElevationConfidence_elev_002_00,
  j2735ElevationConfidence_elev_001_00,
  j2735ElevationConfidence_elev_000_50,
  j2735ElevationConfidence_elev_000_20,
  j2735ElevationConfidence_elev_000_10,
  j2735ElevationConfidence_elev_000_05,
  j2735ElevationConfidence_elev_000_02,
  j2735ElevationConfidence_elev_000_01,
} j2735ElevationConfidence;

extern const ASN1CType asn1_type_j2735ElevationConfidence[];

typedef struct j2735PositionConfidenceSet {
  j2735PositionConfidence pos;
  j2735ElevationConfidence elevation;
} j2735PositionConfidenceSet;


extern const ASN1CType asn1_type_j2735PositionConfidenceSet[];

typedef enum j2735HeadingConfidence {
  j2735HeadingConfidence_unavailable,
  j2735HeadingConfidence_prec10deg,
  j2735HeadingConfidence_prec05deg,
  j2735HeadingConfidence_prec01deg,
  j2735HeadingConfidence_prec0_1deg,
  j2735HeadingConfidence_prec0_05deg,
  j2735HeadingConfidence_prec0_01deg,
  j2735HeadingConfidence_prec0_0125deg,
} j2735HeadingConfidence;

extern const ASN1CType asn1_type_j2735HeadingConfidence[];

typedef enum j2735ThrottleConfidence {
  j2735ThrottleConfidence_unavailable,
  j2735ThrottleConfidence_prec10percent,
  j2735ThrottleConfidence_prec1percent,
  j2735ThrottleConfidence_prec0_5percent,
} j2735ThrottleConfidence;

extern const ASN1CType asn1_type_j2735ThrottleConfidence[];

typedef struct j2735SpeedandHeadingandThrottleConfidence {
  j2735HeadingConfidence heading;
  j2735SpeedConfidence speed;
  j2735ThrottleConfidence throttle;
} j2735SpeedandHeadingandThrottleConfidence;


extern const ASN1CType asn1_type_j2735SpeedandHeadingandThrottleConfidence[];

typedef struct j2735FullPositionVector {
  BOOL utcTime_option;
  j2735DDateTime utcTime;
  j2735Longitude Long;
  j2735Latitude lat;
  BOOL elevation_option;
  j2735Elevation elevation;
  BOOL heading_option;
  j2735Heading heading;
  BOOL speed_option;
  j2735TransmissionAndSpeed speed;
  BOOL posAccuracy_option;
  j2735PositionalAccuracy posAccuracy;
  BOOL timeConfidence_option;
  j2735TimeConfidence timeConfidence;
  BOOL posConfidence_option;
  j2735PositionConfidenceSet posConfidence;
  BOOL speedConfidence_option;
  j2735SpeedandHeadingandThrottleConfidence speedConfidence;
} j2735FullPositionVector;


extern const ASN1CType asn1_type_j2735FullPositionVector[];

typedef ASN1String j2735FurtherInfoID;

extern const ASN1CType asn1_type_j2735FurtherInfoID[];

typedef struct j2735RegionalExtension_10 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_10;


extern const ASN1CType asn1_type_j2735RegionalExtension_10[];

typedef struct j2735RoadSideAlert_2 {
  j2735RegionalExtension_10 *tab;
  size_t count;
} j2735RoadSideAlert_2;

extern const ASN1CType asn1_type_j2735RoadSideAlert_2[];

typedef struct j2735RoadSideAlert {
  j2735MsgCount msgCnt;
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  j2735ITIScodes typeEvent;
  BOOL description_option;
  j2735RoadSideAlert_1 description;
  BOOL priority_option;
  j2735Priority priority;
  BOOL heading_option;
  j2735HeadingSlice heading;
  BOOL extent_option;
  j2735Extent extent;
  BOOL position_option;
  j2735FullPositionVector position;
  BOOL furtherInfoID_option;
  j2735FurtherInfoID furtherInfoID;
  BOOL regional_option;
  j2735RoadSideAlert_2 regional;
} j2735RoadSideAlert;


extern const ASN1CType asn1_type_j2735RoadSideAlert[];

typedef enum j2735ResponseType {
  j2735ResponseType_notInUseOrNotEquipped,
  j2735ResponseType_emergency,
  j2735ResponseType_nonEmergency,
  j2735ResponseType_pursuit,
  j2735ResponseType_stationary,
  j2735ResponseType_slowMoving,
  j2735ResponseType_stopAndGoMovement,
} j2735ResponseType;

extern const ASN1CType asn1_type_j2735ResponseType[];

typedef int j2735SSPindex;

extern const ASN1CType asn1_type_j2735SSPindex[];

typedef enum j2735SirenInUse {
  j2735SirenInUse_unavailable,
  j2735SirenInUse_notInUse,
  j2735SirenInUse_inUse,
  j2735SirenInUse_reserved,
} j2735SirenInUse;

extern const ASN1CType asn1_type_j2735SirenInUse[];

typedef enum j2735LightbarInUse {
  j2735LightbarInUse_unavailable,
  j2735LightbarInUse_notInUse,
  j2735LightbarInUse_inUse,
  j2735LightbarInUse_yellowCautionLights,
  j2735LightbarInUse_schooldBusLights,
  j2735LightbarInUse_arrowSignsActive,
  j2735LightbarInUse_slowMovingVehicle,
  j2735LightbarInUse_freqStops,
} j2735LightbarInUse;

extern const ASN1CType asn1_type_j2735LightbarInUse[];

typedef enum j2735MultiVehicleResponse {
  j2735MultiVehicleResponse_unavailable,
  j2735MultiVehicleResponse_singleVehicle,
  j2735MultiVehicleResponse_multiVehicle,
  j2735MultiVehicleResponse_reserved,
} j2735MultiVehicleResponse;

extern const ASN1CType asn1_type_j2735MultiVehicleResponse[];

typedef ASN1BitString j2735PrivilegedEventFlags;

extern const ASN1CType asn1_type_j2735PrivilegedEventFlags[];

typedef struct j2735PrivilegedEvents {
  j2735SSPindex sspRights;
  j2735PrivilegedEventFlags event;
} j2735PrivilegedEvents;


extern const ASN1CType asn1_type_j2735PrivilegedEvents[];

typedef struct j2735EmergencyDetails {
  j2735SSPindex sspRights;
  j2735SirenInUse sirenUse;
  j2735LightbarInUse lightsUse;
  j2735MultiVehicleResponse multi;
  BOOL events_option;
  j2735PrivilegedEvents events;
  BOOL responseType_option;
  j2735ResponseType responseType;
} j2735EmergencyDetails;


extern const ASN1CType asn1_type_j2735EmergencyDetails[];

typedef int j2735VehicleMass;

extern const ASN1CType asn1_type_j2735VehicleMass[];

typedef enum j2735VehicleType {
  j2735VehicleType_none,
  j2735VehicleType_unknown,
  j2735VehicleType_special,
  j2735VehicleType_moto,
  j2735VehicleType_car,
  j2735VehicleType_carOther,
  j2735VehicleType_bus,
  j2735VehicleType_axleCnt2,
  j2735VehicleType_axleCnt3,
  j2735VehicleType_axleCnt4,
  j2735VehicleType_axleCnt4Trailer,
  j2735VehicleType_axleCnt5Trailer,
  j2735VehicleType_axleCnt6Trailer,
  j2735VehicleType_axleCnt5MultiTrailer,
  j2735VehicleType_axleCnt6MultiTrailer,
  j2735VehicleType_axleCnt7MultiTrailer,
} j2735VehicleType;

extern const ASN1CType asn1_type_j2735VehicleType[];

typedef enum j2735VehicleGroupAffected {
  j2735VehicleGroupAffected_all_vehicles,
  j2735VehicleGroupAffected_bicycles,
  j2735VehicleGroupAffected_motorcycles,
  j2735VehicleGroupAffected_cars,
  j2735VehicleGroupAffected_light_vehicles,
  j2735VehicleGroupAffected_cars_and_light_vehicles,
  j2735VehicleGroupAffected_cars_with_trailers,
  j2735VehicleGroupAffected_cars_with_recreational_trailers,
  j2735VehicleGroupAffected_vehicles_with_trailers,
  j2735VehicleGroupAffected_heavy_vehicles,
  j2735VehicleGroupAffected_trucks,
  j2735VehicleGroupAffected_buses,
  j2735VehicleGroupAffected_articulated_buses,
  j2735VehicleGroupAffected_school_buses,
  j2735VehicleGroupAffected_vehicles_with_semi_trailers,
  j2735VehicleGroupAffected_vehicles_with_double_trailers,
  j2735VehicleGroupAffected_high_profile_vehicles,
  j2735VehicleGroupAffected_wide_vehicles,
  j2735VehicleGroupAffected_long_vehicles,
  j2735VehicleGroupAffected_hazardous_loads,
  j2735VehicleGroupAffected_exceptional_loads,
  j2735VehicleGroupAffected_abnormal_loads,
  j2735VehicleGroupAffected_convoys,
  j2735VehicleGroupAffected_maintenance_vehicles,
  j2735VehicleGroupAffected_delivery_vehicles,
  j2735VehicleGroupAffected_vehicles_with_even_numbered_license_plates,
  j2735VehicleGroupAffected_vehicles_with_odd_numbered_license_plates,
  j2735VehicleGroupAffected_vehicles_with_parking_permits,
  j2735VehicleGroupAffected_vehicles_with_catalytic_converters,
  j2735VehicleGroupAffected_vehicles_without_catalytic_converters,
  j2735VehicleGroupAffected_gas_powered_vehicles,
  j2735VehicleGroupAffected_diesel_powered_vehicles,
  j2735VehicleGroupAffected_lPG_vehicles,
  j2735VehicleGroupAffected_military_convoys,
  j2735VehicleGroupAffected_military_vehicles,
} j2735VehicleGroupAffected;

extern const ASN1CType asn1_type_j2735VehicleGroupAffected[];

typedef enum j2735IncidentResponseEquipment {
  j2735IncidentResponseEquipment_ground_fire_suppression,
  j2735IncidentResponseEquipment_heavy_ground_equipment,
  j2735IncidentResponseEquipment_aircraft,
  j2735IncidentResponseEquipment_marine_equipment,
  j2735IncidentResponseEquipment_support_equipment,
  j2735IncidentResponseEquipment_medical_rescue_unit,
  j2735IncidentResponseEquipment_other,
  j2735IncidentResponseEquipment_ground_fire_suppression_other,
  j2735IncidentResponseEquipment_engine,
  j2735IncidentResponseEquipment_truck_or_aerial,
  j2735IncidentResponseEquipment_quint,
  j2735IncidentResponseEquipment_tanker_pumper_combination,
  j2735IncidentResponseEquipment_brush_truck,
  j2735IncidentResponseEquipment_aircraft_rescue_firefighting,
  j2735IncidentResponseEquipment_heavy_ground_equipment_other,
  j2735IncidentResponseEquipment_dozer_or_plow,
  j2735IncidentResponseEquipment_tractor,
  j2735IncidentResponseEquipment_tanker_or_tender,
  j2735IncidentResponseEquipment_aircraft_other,
  j2735IncidentResponseEquipment_aircraft_fixed_wing_tanker,
  j2735IncidentResponseEquipment_helitanker,
  j2735IncidentResponseEquipment_helicopter,
  j2735IncidentResponseEquipment_marine_equipment_other,
  j2735IncidentResponseEquipment_fire_boat_with_pump,
  j2735IncidentResponseEquipment_boat_no_pump,
  j2735IncidentResponseEquipment_support_apparatus_other,
  j2735IncidentResponseEquipment_breathing_apparatus_support,
  j2735IncidentResponseEquipment_light_and_air_unit,
  j2735IncidentResponseEquipment_medical_rescue_unit_other,
  j2735IncidentResponseEquipment_rescue_unit,
  j2735IncidentResponseEquipment_urban_search_rescue_unit,
  j2735IncidentResponseEquipment_high_angle_rescue,
  j2735IncidentResponseEquipment_crash_fire_rescue,
  j2735IncidentResponseEquipment_bLS_unit,
  j2735IncidentResponseEquipment_aLS_unit,
  j2735IncidentResponseEquipment_mobile_command_post,
  j2735IncidentResponseEquipment_chief_officer_car,
  j2735IncidentResponseEquipment_hAZMAT_unit,
  j2735IncidentResponseEquipment_type_i_hand_crew,
  j2735IncidentResponseEquipment_type_ii_hand_crew,
  j2735IncidentResponseEquipment_privately_owned_vehicle,
  j2735IncidentResponseEquipment_other_apparatus_resource,
  j2735IncidentResponseEquipment_ambulance,
  j2735IncidentResponseEquipment_bomb_squad_van,
  j2735IncidentResponseEquipment_combine_harvester,
  j2735IncidentResponseEquipment_construction_vehicle,
  j2735IncidentResponseEquipment_farm_tractor,
  j2735IncidentResponseEquipment_grass_cutting_machines,
  j2735IncidentResponseEquipment_hAZMAT_containment_tow,
  j2735IncidentResponseEquipment_heavy_tow,
  j2735IncidentResponseEquipment_light_tow,
  j2735IncidentResponseEquipment_flatbed_tow,
  j2735IncidentResponseEquipment_hedge_cutting_machines,
  j2735IncidentResponseEquipment_mobile_crane,
  j2735IncidentResponseEquipment_refuse_collection_vehicle,
  j2735IncidentResponseEquipment_resurfacing_vehicle,
  j2735IncidentResponseEquipment_road_sweeper,
  j2735IncidentResponseEquipment_roadside_litter_collection_crews,
  j2735IncidentResponseEquipment_salvage_vehicle,
  j2735IncidentResponseEquipment_sand_truck,
  j2735IncidentResponseEquipment_snowplow,
  j2735IncidentResponseEquipment_steam_roller,
  j2735IncidentResponseEquipment_swat_team_van,
  j2735IncidentResponseEquipment_track_laying_vehicle,
  j2735IncidentResponseEquipment_unknown_vehicle,
  j2735IncidentResponseEquipment_white_lining_vehicle,
  j2735IncidentResponseEquipment_dump_truck,
  j2735IncidentResponseEquipment_supervisor_vehicle,
  j2735IncidentResponseEquipment_snow_blower,
  j2735IncidentResponseEquipment_rotary_snow_blower,
  j2735IncidentResponseEquipment_road_grader,
  j2735IncidentResponseEquipment_steam_truck,
} j2735IncidentResponseEquipment;

extern const ASN1CType asn1_type_j2735IncidentResponseEquipment[];

typedef enum j2735ResponderGroupAffected {
  j2735ResponderGroupAffected_emergency_vehicle_units,
  j2735ResponderGroupAffected_federal_law_enforcement_units,
  j2735ResponderGroupAffected_state_police_units,
  j2735ResponderGroupAffected_county_police_units,
  j2735ResponderGroupAffected_local_police_units,
  j2735ResponderGroupAffected_ambulance_units,
  j2735ResponderGroupAffected_rescue_units,
  j2735ResponderGroupAffected_fire_units,
  j2735ResponderGroupAffected_hAZMAT_units,
  j2735ResponderGroupAffected_light_tow_unit,
  j2735ResponderGroupAffected_heavy_tow_unit,
  j2735ResponderGroupAffected_freeway_service_patrols,
  j2735ResponderGroupAffected_transportation_response_units,
  j2735ResponderGroupAffected_private_contractor_response_units,
} j2735ResponderGroupAffected;

extern const ASN1CType asn1_type_j2735ResponderGroupAffected[];

typedef struct j2735RegionalExtension_3 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_3;


extern const ASN1CType asn1_type_j2735RegionalExtension_3[];

typedef struct j2735EmergencyVehicleAlert_1 {
  j2735RegionalExtension_3 *tab;
  size_t count;
} j2735EmergencyVehicleAlert_1;

extern const ASN1CType asn1_type_j2735EmergencyVehicleAlert_1[];

typedef struct j2735EmergencyVehicleAlert {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL id_option;
  j2735TemporaryID id;
  j2735RoadSideAlert rsaMsg;
  BOOL responseType_option;
  j2735ResponseType responseType;
  BOOL details_option;
  j2735EmergencyDetails details;
  BOOL mass_option;
  j2735VehicleMass mass;
  BOOL basicType_option;
  j2735VehicleType basicType;
  BOOL vehicleType_option;
  j2735VehicleGroupAffected vehicleType;
  BOOL responseEquip_option;
  j2735IncidentResponseEquipment responseEquip;
  BOOL responderType_option;
  j2735ResponderGroupAffected responderType;
  BOOL regional_option;
  j2735EmergencyVehicleAlert_1 regional;
} j2735EmergencyVehicleAlert;


extern const ASN1CType asn1_type_j2735EmergencyVehicleAlert[];

typedef ASN1BitString j2735GNSSstatus;

extern const ASN1CType asn1_type_j2735GNSSstatus[];

typedef int j2735OffsetLL_B18;

extern const ASN1CType asn1_type_j2735OffsetLL_B18[];

typedef int j2735VertOffset_B12;

extern const ASN1CType asn1_type_j2735VertOffset_B12[];

typedef int j2735TimeOffset;

extern const ASN1CType asn1_type_j2735TimeOffset[];

typedef int j2735CoarseHeading;

extern const ASN1CType asn1_type_j2735CoarseHeading[];

typedef struct j2735PathHistoryPoint {
  j2735OffsetLL_B18 latOffset;
  j2735OffsetLL_B18 lonOffset;
  j2735VertOffset_B12 elevationOffset;
  j2735TimeOffset timeOffset;
  BOOL speed_option;
  j2735Speed speed;
  BOOL posAccuracy_option;
  j2735PositionalAccuracy posAccuracy;
  BOOL heading_option;
  j2735CoarseHeading heading;
} j2735PathHistoryPoint;


extern const ASN1CType asn1_type_j2735PathHistoryPoint[];

typedef struct j2735PathHistoryPointList {
  j2735PathHistoryPoint *tab;
  size_t count;
} j2735PathHistoryPointList;

extern const ASN1CType asn1_type_j2735PathHistoryPointList[];

typedef struct j2735PathHistory {
  BOOL initialPosition_option;
  j2735FullPositionVector initialPosition;
  BOOL currGNSSstatus_option;
  j2735GNSSstatus currGNSSstatus;
  j2735PathHistoryPointList crumbData;
} j2735PathHistory;


extern const ASN1CType asn1_type_j2735PathHistory[];

typedef int j2735RadiusOfCurvature;

extern const ASN1CType asn1_type_j2735RadiusOfCurvature[];

typedef int j2735Confidence;

extern const ASN1CType asn1_type_j2735Confidence[];

typedef struct j2735PathPrediction {
  j2735RadiusOfCurvature radiusOfCurve;
  j2735Confidence confidence;
} j2735PathPrediction;


extern const ASN1CType asn1_type_j2735PathPrediction[];

typedef enum {
  j2735ApproachOrLane_approach,
  j2735ApproachOrLane_lane,
} j2735ApproachOrLane_choice;

typedef struct j2735ApproachOrLane {
  j2735ApproachOrLane_choice choice;
  union {
    j2735ApproachID approach;
    j2735LaneID lane;
  } u;
} j2735ApproachOrLane;

extern const ASN1CType asn1_type_j2735ApproachOrLane[];

typedef ASN1BitString j2735VehicleEventFlags;

extern const ASN1CType asn1_type_j2735VehicleEventFlags[];

typedef struct j2735RegionalExtension_4 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_4;


extern const ASN1CType asn1_type_j2735RegionalExtension_4[];

typedef struct j2735IntersectionCollision_1 {
  j2735RegionalExtension_4 *tab;
  size_t count;
} j2735IntersectionCollision_1;

extern const ASN1CType asn1_type_j2735IntersectionCollision_1[];

typedef struct j2735IntersectionCollision {
  j2735MsgCount msgCnt;
  j2735TemporaryID id;
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL partOne_option;
  j2735BSMcoreData partOne;
  BOOL path_option;
  j2735PathHistory path;
  BOOL pathPrediction_option;
  j2735PathPrediction pathPrediction;
  j2735IntersectionReferenceID intersectionID;
  j2735ApproachOrLane laneNumber;
  j2735VehicleEventFlags eventFlag;
  BOOL regional_option;
  j2735IntersectionCollision_1 regional;
} j2735IntersectionCollision;


extern const ASN1CType asn1_type_j2735IntersectionCollision[];

typedef enum j2735NMEA_Revision {
  j2735NMEA_Revision_unknown,
  j2735NMEA_Revision_reserved,
  j2735NMEA_Revision_rev1,
  j2735NMEA_Revision_rev2,
  j2735NMEA_Revision_rev3,
  j2735NMEA_Revision_rev4,
  j2735NMEA_Revision_rev5,
} j2735NMEA_Revision;

extern const ASN1CType asn1_type_j2735NMEA_Revision[];

typedef int j2735NMEA_MsgType;

extern const ASN1CType asn1_type_j2735NMEA_MsgType[];

typedef int j2735ObjectCount;

extern const ASN1CType asn1_type_j2735ObjectCount[];

typedef ASN1String j2735NMEA_Payload;

extern const ASN1CType asn1_type_j2735NMEA_Payload[];

typedef struct j2735RegionalExtension_6 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_6;


extern const ASN1CType asn1_type_j2735RegionalExtension_6[];

typedef struct j2735NMEAcorrections_1 {
  j2735RegionalExtension_6 *tab;
  size_t count;
} j2735NMEAcorrections_1;

extern const ASN1CType asn1_type_j2735NMEAcorrections_1[];

typedef struct j2735NMEAcorrections {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL rev_option;
  j2735NMEA_Revision rev;
  BOOL msg_option;
  j2735NMEA_MsgType msg;
  BOOL wdCount_option;
  j2735ObjectCount wdCount;
  j2735NMEA_Payload payload;
  BOOL regional_option;
  j2735NMEAcorrections_1 regional;
} j2735NMEAcorrections;


extern const ASN1CType asn1_type_j2735NMEAcorrections[];

typedef struct j2735Sample {
  int sampleStart;
  int sampleEnd;
} j2735Sample;


extern const ASN1CType asn1_type_j2735Sample[];

typedef int j2735TermTime;

extern const ASN1CType asn1_type_j2735TermTime[];

typedef int j2735TermDistance;

extern const ASN1CType asn1_type_j2735TermDistance[];

typedef enum {
  j2735ProbeDataManagement_1_termtime,
  j2735ProbeDataManagement_1_termDistance,
} j2735ProbeDataManagement_1_choice;

typedef struct j2735ProbeDataManagement_1 {
  j2735ProbeDataManagement_1_choice choice;
  union {
    j2735TermTime termtime;
    j2735TermDistance termDistance;
  } u;
} j2735ProbeDataManagement_1;

extern const ASN1CType asn1_type_j2735ProbeDataManagement_1[];

typedef int j2735GrossSpeed;

extern const ASN1CType asn1_type_j2735GrossSpeed[];

typedef int j2735SecondOfTime;

extern const ASN1CType asn1_type_j2735SecondOfTime[];

typedef struct j2735SnapshotTime {
  j2735GrossSpeed speed1;
  j2735SecondOfTime time1;
  j2735GrossSpeed speed2;
  j2735SecondOfTime time2;
} j2735SnapshotTime;


extern const ASN1CType asn1_type_j2735SnapshotTime[];

typedef int j2735GrossDistance;

extern const ASN1CType asn1_type_j2735GrossDistance[];

typedef struct j2735SnapshotDistance {
  j2735GrossDistance distance1;
  j2735GrossSpeed speed1;
  j2735GrossDistance distance2;
  j2735GrossSpeed speed2;
} j2735SnapshotDistance;


extern const ASN1CType asn1_type_j2735SnapshotDistance[];

typedef enum {
  j2735ProbeDataManagement_2_snapshotTime,
  j2735ProbeDataManagement_2_snapshotDistance,
} j2735ProbeDataManagement_2_choice;

typedef struct j2735ProbeDataManagement_2 {
  j2735ProbeDataManagement_2_choice choice;
  union {
    j2735SnapshotTime snapshotTime;
    j2735SnapshotDistance snapshotDistance;
  } u;
} j2735ProbeDataManagement_2;

extern const ASN1CType asn1_type_j2735ProbeDataManagement_2[];

typedef enum j2735VehicleStatusDeviceTypeTag {
  j2735VehicleStatusDeviceTypeTag_unknown,
  j2735VehicleStatusDeviceTypeTag_lights,
  j2735VehicleStatusDeviceTypeTag_wipers,
  j2735VehicleStatusDeviceTypeTag_brakes,
  j2735VehicleStatusDeviceTypeTag_stab,
  j2735VehicleStatusDeviceTypeTag_trac,
  j2735VehicleStatusDeviceTypeTag_albs,
  j2735VehicleStatusDeviceTypeTag_sunS,
  j2735VehicleStatusDeviceTypeTag_rainS,
  j2735VehicleStatusDeviceTypeTag_airTemp,
  j2735VehicleStatusDeviceTypeTag_steering,
  j2735VehicleStatusDeviceTypeTag_vertAccelThres,
  j2735VehicleStatusDeviceTypeTag_vertAccel,
  j2735VehicleStatusDeviceTypeTag_hozAccelLong,
  j2735VehicleStatusDeviceTypeTag_hozAccelLat,
  j2735VehicleStatusDeviceTypeTag_hozAccelCon,
  j2735VehicleStatusDeviceTypeTag_accel4way,
  j2735VehicleStatusDeviceTypeTag_confidenceSet,
  j2735VehicleStatusDeviceTypeTag_obDist,
  j2735VehicleStatusDeviceTypeTag_obDirect,
  j2735VehicleStatusDeviceTypeTag_yaw,
  j2735VehicleStatusDeviceTypeTag_yawRateCon,
  j2735VehicleStatusDeviceTypeTag_dateTime,
  j2735VehicleStatusDeviceTypeTag_fullPos,
  j2735VehicleStatusDeviceTypeTag_position2D,
  j2735VehicleStatusDeviceTypeTag_position3D,
  j2735VehicleStatusDeviceTypeTag_vehicle,
  j2735VehicleStatusDeviceTypeTag_speedHeadC,
  j2735VehicleStatusDeviceTypeTag_speedC,
} j2735VehicleStatusDeviceTypeTag;

extern const ASN1CType asn1_type_j2735VehicleStatusDeviceTypeTag[];

typedef struct j2735VehicleStatusRequest {
  j2735VehicleStatusDeviceTypeTag dataType;
  BOOL subType_option;
  int subType;
  BOOL sendOnLessThenValue_option;
  int sendOnLessThenValue;
  BOOL sendOnMoreThenValue_option;
  int sendOnMoreThenValue;
  BOOL sendAll_option;
  BOOL sendAll;
} j2735VehicleStatusRequest;


extern const ASN1CType asn1_type_j2735VehicleStatusRequest[];

typedef struct j2735VehicleStatusRequestList {
  j2735VehicleStatusRequest *tab;
  size_t count;
} j2735VehicleStatusRequestList;

extern const ASN1CType asn1_type_j2735VehicleStatusRequestList[];

typedef struct j2735RegionalExtension_8 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_8;


extern const ASN1CType asn1_type_j2735RegionalExtension_8[];

typedef struct j2735ProbeDataManagement_3 {
  j2735RegionalExtension_8 *tab;
  size_t count;
} j2735ProbeDataManagement_3;

extern const ASN1CType asn1_type_j2735ProbeDataManagement_3[];

typedef struct j2735ProbeDataManagement {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  j2735Sample sample;
  j2735HeadingSlice directions;
  j2735ProbeDataManagement_1 term;
  j2735ProbeDataManagement_2 snapshot;
  j2735SecondOfTime txInterval;
  BOOL dataElements_option;
  j2735VehicleStatusRequestList dataElements;
  BOOL regional_option;
  j2735ProbeDataManagement_3 regional;
} j2735ProbeDataManagement;


extern const ASN1CType asn1_type_j2735ProbeDataManagement[];

typedef int j2735ProbeSegmentNumber;

extern const ASN1CType asn1_type_j2735ProbeSegmentNumber[];

typedef ASN1String j2735VINstring;

extern const ASN1CType asn1_type_j2735VINstring[];

typedef unsigned int j2735StationID;

extern const ASN1CType asn1_type_j2735StationID[];

typedef enum {
  j2735VehicleID_entityID,
  j2735VehicleID_stationID,
} j2735VehicleID_choice;

typedef struct j2735VehicleID {
  j2735VehicleID_choice choice;
  union {
    j2735TemporaryID entityID;
    j2735StationID stationID;
  } u;
} j2735VehicleID;

extern const ASN1CType asn1_type_j2735VehicleID[];

typedef enum {
  j2735VehicleIdent_1_vGroup,
  j2735VehicleIdent_1_rGroup,
  j2735VehicleIdent_1_rEquip,
} j2735VehicleIdent_1_choice;

typedef struct j2735VehicleIdent_1 {
  j2735VehicleIdent_1_choice choice;
  union {
    j2735VehicleGroupAffected vGroup;
    j2735ResponderGroupAffected rGroup;
    j2735IncidentResponseEquipment rEquip;
  } u;
} j2735VehicleIdent_1;

extern const ASN1CType asn1_type_j2735VehicleIdent_1[];

typedef struct j2735VehicleIdent {
  BOOL name_option;
  j2735DescriptiveName name;
  BOOL vin_option;
  j2735VINstring vin;
  BOOL ownerCode_option;
  ASN1String ownerCode;
  BOOL id_option;
  j2735VehicleID id;
  BOOL vehicleType_option;
  j2735VehicleType vehicleType;
  BOOL vehicleClass_option;
  j2735VehicleIdent_1 vehicleClass;
} j2735VehicleIdent;


extern const ASN1CType asn1_type_j2735VehicleIdent[];

typedef int j2735BasicVehicleClass;

extern const ASN1CType asn1_type_j2735BasicVehicleClass[];

typedef enum j2735BasicVehicleRole {
  j2735BasicVehicleRole_basicVehicle,
  j2735BasicVehicleRole_publicTransport,
  j2735BasicVehicleRole_specialTransport,
  j2735BasicVehicleRole_dangerousGoods,
  j2735BasicVehicleRole_roadWork,
  j2735BasicVehicleRole_roadRescue,
  j2735BasicVehicleRole_emergency,
  j2735BasicVehicleRole_safetyCar,
  j2735BasicVehicleRole_none_unknown,
  j2735BasicVehicleRole_truck,
  j2735BasicVehicleRole_motorcycle,
  j2735BasicVehicleRole_roadSideSource,
  j2735BasicVehicleRole_police,
  j2735BasicVehicleRole_fire,
  j2735BasicVehicleRole_ambulance,
  j2735BasicVehicleRole_dot,
  j2735BasicVehicleRole_transit,
  j2735BasicVehicleRole_slowMoving,
  j2735BasicVehicleRole_stopNgo,
  j2735BasicVehicleRole_cyclist,
  j2735BasicVehicleRole_pedestrian,
  j2735BasicVehicleRole_nonMotorized,
  j2735BasicVehicleRole_military,
} j2735BasicVehicleRole;

extern const ASN1CType asn1_type_j2735BasicVehicleRole[];

typedef int j2735Iso3833VehicleType;

extern const ASN1CType asn1_type_j2735Iso3833VehicleType[];

typedef int j2735FuelType;

extern const ASN1CType asn1_type_j2735FuelType[];

typedef struct j2735RegionalExtension_60 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_60;


extern const ASN1CType asn1_type_j2735RegionalExtension_60[];

typedef struct j2735VehicleClassification_1 {
  j2735RegionalExtension_60 *tab;
  size_t count;
} j2735VehicleClassification_1;

extern const ASN1CType asn1_type_j2735VehicleClassification_1[];

typedef struct j2735VehicleClassification {
  BOOL keyType_option;
  j2735BasicVehicleClass keyType;
  BOOL role_option;
  j2735BasicVehicleRole role;
  BOOL iso3883_option;
  j2735Iso3833VehicleType iso3883;
  BOOL hpmsType_option;
  j2735VehicleType hpmsType;
  BOOL vehicleType_option;
  j2735VehicleGroupAffected vehicleType;
  BOOL responseEquip_option;
  j2735IncidentResponseEquipment responseEquip;
  BOOL responderType_option;
  j2735ResponderGroupAffected responderType;
  BOOL fuelType_option;
  j2735FuelType fuelType;
  BOOL regional_option;
  j2735VehicleClassification_1 regional;
} j2735VehicleClassification;


extern const ASN1CType asn1_type_j2735VehicleClassification[];

typedef ASN1BitString j2735ExteriorLights;

extern const ASN1CType asn1_type_j2735ExteriorLights[];

typedef struct j2735VehicleSafetyExtensions {
  BOOL events_option;
  j2735VehicleEventFlags events;
  BOOL pathHistory_option;
  j2735PathHistory pathHistory;
  BOOL pathPrediction_option;
  j2735PathPrediction pathPrediction;
  BOOL lights_option;
  j2735ExteriorLights lights;
} j2735VehicleSafetyExtensions;


extern const ASN1CType asn1_type_j2735VehicleSafetyExtensions[];

typedef enum j2735WiperStatus {
  j2735WiperStatus_unavailable,
  j2735WiperStatus_off,
  j2735WiperStatus_intermittent,
  j2735WiperStatus_low,
  j2735WiperStatus_high,
  j2735WiperStatus_washerInUse,
  j2735WiperStatus_automaticPresent,
} j2735WiperStatus;

extern const ASN1CType asn1_type_j2735WiperStatus[];

typedef int j2735WiperRate;

extern const ASN1CType asn1_type_j2735WiperRate[];

typedef struct j2735WiperSet {
  j2735WiperStatus statusFront;
  j2735WiperRate rateFront;
  BOOL statusRear_option;
  j2735WiperStatus statusRear;
  BOOL rateRear_option;
  j2735WiperRate rateRear;
} j2735WiperSet;


extern const ASN1CType asn1_type_j2735WiperSet[];

typedef enum j2735BrakeAppliedPressure {
  j2735BrakeAppliedPressure_unavailable,
  j2735BrakeAppliedPressure_minPressure,
  j2735BrakeAppliedPressure_bkLvl_2,
  j2735BrakeAppliedPressure_bkLvl_3,
  j2735BrakeAppliedPressure_bkLvl_4,
  j2735BrakeAppliedPressure_bkLvl_5,
  j2735BrakeAppliedPressure_bkLvl_6,
  j2735BrakeAppliedPressure_bkLvl_7,
  j2735BrakeAppliedPressure_bkLvl_8,
  j2735BrakeAppliedPressure_bkLvl_9,
  j2735BrakeAppliedPressure_bkLvl_10,
  j2735BrakeAppliedPressure_bkLvl_11,
  j2735BrakeAppliedPressure_bkLvl_12,
  j2735BrakeAppliedPressure_bkLvl_13,
  j2735BrakeAppliedPressure_bkLvl_14,
  j2735BrakeAppliedPressure_maxPressure,
} j2735BrakeAppliedPressure;

extern const ASN1CType asn1_type_j2735BrakeAppliedPressure[];

typedef int j2735CoefficientOfFriction;

extern const ASN1CType asn1_type_j2735CoefficientOfFriction[];

typedef int j2735SunSensor;

extern const ASN1CType asn1_type_j2735SunSensor[];

typedef enum j2735RainSensor {
  j2735RainSensor_none,
  j2735RainSensor_lightMist,
  j2735RainSensor_heavyMist,
  j2735RainSensor_lightRainOrDrizzle,
  j2735RainSensor_rain,
  j2735RainSensor_moderateRain,
  j2735RainSensor_heavyRain,
  j2735RainSensor_heavyDownpour,
} j2735RainSensor;

extern const ASN1CType asn1_type_j2735RainSensor[];

typedef int j2735AmbientAirTemperature;

extern const ASN1CType asn1_type_j2735AmbientAirTemperature[];

typedef int j2735AmbientAirPressure;

extern const ASN1CType asn1_type_j2735AmbientAirPressure[];

typedef enum j2735SteeringWheelAngleConfidence {
  j2735SteeringWheelAngleConfidence_unavailable,
  j2735SteeringWheelAngleConfidence_prec2deg,
  j2735SteeringWheelAngleConfidence_prec1deg,
  j2735SteeringWheelAngleConfidence_prec0_02deg,
} j2735SteeringWheelAngleConfidence;

extern const ASN1CType asn1_type_j2735SteeringWheelAngleConfidence[];

typedef int j2735SteeringWheelAngleRateOfChange;

extern const ASN1CType asn1_type_j2735SteeringWheelAngleRateOfChange[];

typedef int j2735DrivingWheelAngle;

extern const ASN1CType asn1_type_j2735DrivingWheelAngle[];

typedef struct j2735VehicleStatus_1 {
  j2735SteeringWheelAngle angle;
  BOOL confidence_option;
  j2735SteeringWheelAngleConfidence confidence;
  BOOL rate_option;
  j2735SteeringWheelAngleRateOfChange rate;
  BOOL wheels_option;
  j2735DrivingWheelAngle wheels;
} j2735VehicleStatus_1;


extern const ASN1CType asn1_type_j2735VehicleStatus_1[];

typedef ASN1BitString j2735VerticalAccelerationThreshold;

extern const ASN1CType asn1_type_j2735VerticalAccelerationThreshold[];

typedef enum j2735YawRateConfidence {
  j2735YawRateConfidence_unavailable,
  j2735YawRateConfidence_degSec_100_00,
  j2735YawRateConfidence_degSec_010_00,
  j2735YawRateConfidence_degSec_005_00,
  j2735YawRateConfidence_degSec_001_00,
  j2735YawRateConfidence_degSec_000_10,
  j2735YawRateConfidence_degSec_000_05,
  j2735YawRateConfidence_degSec_000_01,
} j2735YawRateConfidence;

extern const ASN1CType asn1_type_j2735YawRateConfidence[];

typedef enum j2735AccelerationConfidence {
  j2735AccelerationConfidence_unavailable,
  j2735AccelerationConfidence_accl_100_00,
  j2735AccelerationConfidence_accl_010_00,
  j2735AccelerationConfidence_accl_005_00,
  j2735AccelerationConfidence_accl_001_00,
  j2735AccelerationConfidence_accl_000_10,
  j2735AccelerationConfidence_accl_000_05,
  j2735AccelerationConfidence_accl_000_01,
} j2735AccelerationConfidence;

extern const ASN1CType asn1_type_j2735AccelerationConfidence[];

typedef struct j2735AccelSteerYawRateConfidence {
  j2735YawRateConfidence yawRate;
  j2735AccelerationConfidence acceleration;
  j2735SteeringWheelAngleConfidence steeringWheelAngle;
} j2735AccelSteerYawRateConfidence;


extern const ASN1CType asn1_type_j2735AccelSteerYawRateConfidence[];

typedef struct j2735ConfidenceSet {
  BOOL accelConfidence_option;
  j2735AccelSteerYawRateConfidence accelConfidence;
  BOOL speedConfidence_option;
  j2735SpeedandHeadingandThrottleConfidence speedConfidence;
  BOOL timeConfidence_option;
  j2735TimeConfidence timeConfidence;
  BOOL posConfidence_option;
  j2735PositionConfidenceSet posConfidence;
  BOOL steerConfidence_option;
  j2735SteeringWheelAngleConfidence steerConfidence;
  BOOL headingConfidence_option;
  j2735HeadingConfidence headingConfidence;
  BOOL throttleConfidence_option;
  j2735ThrottleConfidence throttleConfidence;
} j2735ConfidenceSet;


extern const ASN1CType asn1_type_j2735ConfidenceSet[];

typedef struct j2735VehicleStatus_2 {
  BOOL accel4way_option;
  j2735AccelerationSet4Way accel4way;
  BOOL vertAccelThres_option;
  j2735VerticalAccelerationThreshold vertAccelThres;
  BOOL yawRateCon_option;
  j2735YawRateConfidence yawRateCon;
  BOOL hozAccelCon_option;
  j2735AccelerationConfidence hozAccelCon;
  BOOL confidenceSet_option;
  j2735ConfidenceSet confidenceSet;
} j2735VehicleStatus_2;


extern const ASN1CType asn1_type_j2735VehicleStatus_2[];

typedef int j2735ObstacleDistance;

extern const ASN1CType asn1_type_j2735ObstacleDistance[];

typedef struct j2735VehicleStatus_3 {
  j2735ObstacleDistance obDist;
  j2735Angle obDirect;
  j2735DDateTime dateTime;
} j2735VehicleStatus_3;


extern const ASN1CType asn1_type_j2735VehicleStatus_3[];

typedef int j2735ThrottlePosition;

extern const ASN1CType asn1_type_j2735ThrottlePosition[];

typedef int j2735VehicleHeight;

extern const ASN1CType asn1_type_j2735VehicleHeight[];

typedef int j2735BumperHeight;

extern const ASN1CType asn1_type_j2735BumperHeight[];

typedef struct j2735BumperHeights {
  j2735BumperHeight front;
  j2735BumperHeight rear;
} j2735BumperHeights;


extern const ASN1CType asn1_type_j2735BumperHeights[];

typedef int j2735TrailerWeight;

extern const ASN1CType asn1_type_j2735TrailerWeight[];

typedef struct j2735VehicleStatus_4 {
  j2735VehicleHeight height;
  j2735BumperHeights bumpers;
  j2735VehicleMass mass;
  j2735TrailerWeight trailerWeight;
  j2735VehicleType type;
} j2735VehicleStatus_4;


extern const ASN1CType asn1_type_j2735VehicleStatus_4[];

typedef int j2735TireLocation;

extern const ASN1CType asn1_type_j2735TireLocation[];

typedef int j2735TirePressure;

extern const ASN1CType asn1_type_j2735TirePressure[];

typedef int j2735TireTemp;

extern const ASN1CType asn1_type_j2735TireTemp[];

typedef enum j2735WheelSensorStatus {
  j2735WheelSensorStatus_off,
  j2735WheelSensorStatus_on,
  j2735WheelSensorStatus_notDefined,
  j2735WheelSensorStatus_notSupported,
} j2735WheelSensorStatus;

extern const ASN1CType asn1_type_j2735WheelSensorStatus[];

typedef enum j2735WheelEndElectFault {
  j2735WheelEndElectFault_isOk,
  j2735WheelEndElectFault_isNotDefined,
  j2735WheelEndElectFault_isError,
  j2735WheelEndElectFault_isNotSupported,
} j2735WheelEndElectFault;

extern const ASN1CType asn1_type_j2735WheelEndElectFault[];

typedef int j2735TireLeakageRate;

extern const ASN1CType asn1_type_j2735TireLeakageRate[];

typedef enum j2735TirePressureThresholdDetection {
  j2735TirePressureThresholdDetection_noData,
  j2735TirePressureThresholdDetection_overPressure,
  j2735TirePressureThresholdDetection_noWarningPressure,
  j2735TirePressureThresholdDetection_underPressure,
  j2735TirePressureThresholdDetection_extremeUnderPressure,
  j2735TirePressureThresholdDetection_undefined,
  j2735TirePressureThresholdDetection_errorIndicator,
  j2735TirePressureThresholdDetection_notAvailable,
} j2735TirePressureThresholdDetection;

extern const ASN1CType asn1_type_j2735TirePressureThresholdDetection[];

typedef struct j2735TireData {
  BOOL location_option;
  j2735TireLocation location;
  BOOL pressure_option;
  j2735TirePressure pressure;
  BOOL temp_option;
  j2735TireTemp temp;
  BOOL wheelSensorStatus_option;
  j2735WheelSensorStatus wheelSensorStatus;
  BOOL wheelEndElectFault_option;
  j2735WheelEndElectFault wheelEndElectFault;
  BOOL leakageRate_option;
  j2735TireLeakageRate leakageRate;
  BOOL detection_option;
  j2735TirePressureThresholdDetection detection;
} j2735TireData;


extern const ASN1CType asn1_type_j2735TireData[];

typedef struct j2735TireDataList {
  j2735TireData *tab;
  size_t count;
} j2735TireDataList;

extern const ASN1CType asn1_type_j2735TireDataList[];

typedef int j2735AxleLocation;

extern const ASN1CType asn1_type_j2735AxleLocation[];

typedef int j2735AxleWeight;

extern const ASN1CType asn1_type_j2735AxleWeight[];

typedef struct j2735AxleWeightSet {
  BOOL location_option;
  j2735AxleLocation location;
  BOOL weight_option;
  j2735AxleWeight weight;
} j2735AxleWeightSet;


extern const ASN1CType asn1_type_j2735AxleWeightSet[];

typedef struct j2735AxleWeightList {
  j2735AxleWeightSet *tab;
  size_t count;
} j2735AxleWeightList;

extern const ASN1CType asn1_type_j2735AxleWeightList[];

typedef int j2735CargoWeight;

extern const ASN1CType asn1_type_j2735CargoWeight[];

typedef int j2735SteeringAxleTemperature;

extern const ASN1CType asn1_type_j2735SteeringAxleTemperature[];

typedef int j2735DriveAxleLocation;

extern const ASN1CType asn1_type_j2735DriveAxleLocation[];

typedef int j2735DriveAxleLiftAirPressure;

extern const ASN1CType asn1_type_j2735DriveAxleLiftAirPressure[];

typedef int j2735DriveAxleTemperature;

extern const ASN1CType asn1_type_j2735DriveAxleTemperature[];

typedef int j2735DriveAxleLubePressure;

extern const ASN1CType asn1_type_j2735DriveAxleLubePressure[];

typedef int j2735SteeringAxleLubePressure;

extern const ASN1CType asn1_type_j2735SteeringAxleLubePressure[];

typedef struct j2735J1939data {
  BOOL tires_option;
  j2735TireDataList tires;
  BOOL axles_option;
  j2735AxleWeightList axles;
  BOOL trailerWeight_option;
  j2735TrailerWeight trailerWeight;
  BOOL cargoWeight_option;
  j2735CargoWeight cargoWeight;
  BOOL steeringAxleTemperature_option;
  j2735SteeringAxleTemperature steeringAxleTemperature;
  BOOL driveAxleLocation_option;
  j2735DriveAxleLocation driveAxleLocation;
  BOOL driveAxleLiftAirPressure_option;
  j2735DriveAxleLiftAirPressure driveAxleLiftAirPressure;
  BOOL driveAxleTemperature_option;
  j2735DriveAxleTemperature driveAxleTemperature;
  BOOL driveAxleLubePressure_option;
  j2735DriveAxleLubePressure driveAxleLubePressure;
  BOOL steeringAxleLubePressure_option;
  j2735SteeringAxleLubePressure steeringAxleLubePressure;
} j2735J1939data;


extern const ASN1CType asn1_type_j2735J1939data[];

typedef enum j2735EssPrecipYesNo {
  j2735EssPrecipYesNo_precip,
  j2735EssPrecipYesNo_noPrecip,
  j2735EssPrecipYesNo_error,
} j2735EssPrecipYesNo;

extern const ASN1CType asn1_type_j2735EssPrecipYesNo[];

typedef int j2735EssPrecipRate;

extern const ASN1CType asn1_type_j2735EssPrecipRate[];

typedef enum j2735EssPrecipSituation {
  j2735EssPrecipSituation_other,
  j2735EssPrecipSituation_unknown,
  j2735EssPrecipSituation_noPrecipitation,
  j2735EssPrecipSituation_unidentifiedSlight,
  j2735EssPrecipSituation_unidentifiedModerate,
  j2735EssPrecipSituation_unidentifiedHeavy,
  j2735EssPrecipSituation_snowSlight,
  j2735EssPrecipSituation_snowModerate,
  j2735EssPrecipSituation_snowHeavy,
  j2735EssPrecipSituation_rainSlight,
  j2735EssPrecipSituation_rainModerate,
  j2735EssPrecipSituation_rainHeavy,
  j2735EssPrecipSituation_frozenPrecipitationSlight,
  j2735EssPrecipSituation_frozenPrecipitationModerate,
  j2735EssPrecipSituation_frozenPrecipitationHeavy,
} j2735EssPrecipSituation;

extern const ASN1CType asn1_type_j2735EssPrecipSituation[];

typedef int j2735EssSolarRadiation;

extern const ASN1CType asn1_type_j2735EssSolarRadiation[];

typedef int j2735EssMobileFriction;

extern const ASN1CType asn1_type_j2735EssMobileFriction[];

typedef struct j2735VehicleStatus_5 {
  j2735EssPrecipYesNo isRaining;
  BOOL rainRate_option;
  j2735EssPrecipRate rainRate;
  BOOL precipSituation_option;
  j2735EssPrecipSituation precipSituation;
  BOOL solarRadiation_option;
  j2735EssSolarRadiation solarRadiation;
  BOOL friction_option;
  j2735EssMobileFriction friction;
} j2735VehicleStatus_5;


extern const ASN1CType asn1_type_j2735VehicleStatus_5[];

typedef struct j2735VehicleStatus {
  BOOL lights_option;
  j2735ExteriorLights lights;
  BOOL lightBar_option;
  j2735LightbarInUse lightBar;
  BOOL wipers_option;
  j2735WiperSet wipers;
  BOOL brakeStatus_option;
  j2735BrakeSystemStatus brakeStatus;
  BOOL brakePressure_option;
  j2735BrakeAppliedPressure brakePressure;
  BOOL roadFriction_option;
  j2735CoefficientOfFriction roadFriction;
  BOOL sunData_option;
  j2735SunSensor sunData;
  BOOL rainData_option;
  j2735RainSensor rainData;
  BOOL airTemp_option;
  j2735AmbientAirTemperature airTemp;
  BOOL airPres_option;
  j2735AmbientAirPressure airPres;
  BOOL steering_option;
  j2735VehicleStatus_1 steering;
  BOOL accelSets_option;
  j2735VehicleStatus_2 accelSets;
  BOOL object_option;
  j2735VehicleStatus_3 object;
  BOOL fullPos_option;
  j2735FullPositionVector fullPos;
  BOOL throttlePos_option;
  j2735ThrottlePosition throttlePos;
  BOOL speedHeadC_option;
  j2735SpeedandHeadingandThrottleConfidence speedHeadC;
  BOOL speedC_option;
  j2735SpeedConfidence speedC;
  BOOL vehicleData_option;
  j2735VehicleStatus_4 vehicleData;
  BOOL vehicleIdent_option;
  j2735VehicleIdent vehicleIdent;
  BOOL j1939data_option;
  j2735J1939data j1939data;
  BOOL weatherReport_option;
  j2735VehicleStatus_5 weatherReport;
  BOOL gnssStatus_option;
  j2735GNSSstatus gnssStatus;
} j2735VehicleStatus;


extern const ASN1CType asn1_type_j2735VehicleStatus[];

typedef struct j2735Snapshot {
  j2735FullPositionVector thePosition;
  BOOL safetyExt_option;
  j2735VehicleSafetyExtensions safetyExt;
  BOOL dataSet_option;
  j2735VehicleStatus dataSet;
} j2735Snapshot;


extern const ASN1CType asn1_type_j2735Snapshot[];

typedef struct j2735ProbeVehicleData_1 {
  j2735Snapshot *tab;
  size_t count;
} j2735ProbeVehicleData_1;

extern const ASN1CType asn1_type_j2735ProbeVehicleData_1[];

typedef struct j2735RegionalExtension_9 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_9;


extern const ASN1CType asn1_type_j2735RegionalExtension_9[];

typedef struct j2735ProbeVehicleData_2 {
  j2735RegionalExtension_9 *tab;
  size_t count;
} j2735ProbeVehicleData_2;

extern const ASN1CType asn1_type_j2735ProbeVehicleData_2[];

typedef struct j2735ProbeVehicleData {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL segNum_option;
  j2735ProbeSegmentNumber segNum;
  BOOL probeID_option;
  j2735VehicleIdent probeID;
  j2735FullPositionVector startVector;
  j2735VehicleClassification vehicleType;
  j2735ProbeVehicleData_1 snapshots;
  BOOL regional_option;
  j2735ProbeVehicleData_2 regional;
} j2735ProbeVehicleData;


extern const ASN1CType asn1_type_j2735ProbeVehicleData[];

typedef enum j2735RTCM_Revision {
  j2735RTCM_Revision_unknown,
  j2735RTCM_Revision_rtcmRev2,
  j2735RTCM_Revision_rtcmRev3,
  j2735RTCM_Revision_reserved,
} j2735RTCM_Revision;

extern const ASN1CType asn1_type_j2735RTCM_Revision[];

typedef int j2735Offset_B09;

extern const ASN1CType asn1_type_j2735Offset_B09[];

typedef struct j2735AntennaOffsetSet {
  j2735Offset_B12 antOffsetX;
  j2735Offset_B09 antOffsetY;
  j2735Offset_B10 antOffsetZ;
} j2735AntennaOffsetSet;


extern const ASN1CType asn1_type_j2735AntennaOffsetSet[];

typedef struct j2735RTCMheader {
  j2735GNSSstatus status;
  j2735AntennaOffsetSet offsetSet;
} j2735RTCMheader;


extern const ASN1CType asn1_type_j2735RTCMheader[];

typedef ASN1String j2735RTCMmessage;

extern const ASN1CType asn1_type_j2735RTCMmessage[];

typedef struct j2735RTCMmessageList {
  j2735RTCMmessage *tab;
  size_t count;
} j2735RTCMmessageList;

extern const ASN1CType asn1_type_j2735RTCMmessageList[];

typedef struct j2735RegionalExtension_11 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_11;


extern const ASN1CType asn1_type_j2735RegionalExtension_11[];

typedef struct j2735RTCMcorrections_1 {
  j2735RegionalExtension_11 *tab;
  size_t count;
} j2735RTCMcorrections_1;

extern const ASN1CType asn1_type_j2735RTCMcorrections_1[];

typedef struct j2735RTCMcorrections {
  j2735MsgCount msgCnt;
  j2735RTCM_Revision rev;
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL anchorPoint_option;
  j2735FullPositionVector anchorPoint;
  BOOL rtcmHeader_option;
  j2735RTCMheader rtcmHeader;
  j2735RTCMmessageList msgs;
  BOOL regional_option;
  j2735RTCMcorrections_1 regional;
} j2735RTCMcorrections;


extern const ASN1CType asn1_type_j2735RTCMcorrections[];

typedef int j2735RequestID;

extern const ASN1CType asn1_type_j2735RequestID[];

typedef enum j2735PriorityRequestType {
  j2735PriorityRequestType_priorityRequestTypeReserved,
  j2735PriorityRequestType_priorityRequest,
  j2735PriorityRequestType_priorityRequestUpdate,
  j2735PriorityRequestType_priorityCancellation,
} j2735PriorityRequestType;

extern const ASN1CType asn1_type_j2735PriorityRequestType[];

typedef enum {
  j2735IntersectionAccessPoint_lane,
  j2735IntersectionAccessPoint_approach,
  j2735IntersectionAccessPoint_connection,
} j2735IntersectionAccessPoint_choice;

typedef struct j2735IntersectionAccessPoint {
  j2735IntersectionAccessPoint_choice choice;
  union {
    j2735LaneID lane;
    j2735ApproachID approach;
    j2735LaneConnectionID connection;
  } u;
} j2735IntersectionAccessPoint;

extern const ASN1CType asn1_type_j2735IntersectionAccessPoint[];

typedef struct j2735RegionalExtension_56 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_56;


extern const ASN1CType asn1_type_j2735RegionalExtension_56[];

typedef struct j2735SignalRequest_1 {
  j2735RegionalExtension_56 *tab;
  size_t count;
} j2735SignalRequest_1;

extern const ASN1CType asn1_type_j2735SignalRequest_1[];

typedef struct j2735SignalRequest {
  j2735IntersectionReferenceID id;
  j2735RequestID requestID;
  j2735PriorityRequestType requestType;
  j2735IntersectionAccessPoint inBoundLane;
  BOOL outBoundLane_option;
  j2735IntersectionAccessPoint outBoundLane;
  BOOL regional_option;
  j2735SignalRequest_1 regional;
} j2735SignalRequest;


extern const ASN1CType asn1_type_j2735SignalRequest[];

typedef struct j2735RegionalExtension_55 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_55;


extern const ASN1CType asn1_type_j2735RegionalExtension_55[];

typedef struct j2735SignalRequestPackage_1 {
  j2735RegionalExtension_55 *tab;
  size_t count;
} j2735SignalRequestPackage_1;

extern const ASN1CType asn1_type_j2735SignalRequestPackage_1[];

typedef struct j2735SignalRequestPackage {
  j2735SignalRequest request;
  BOOL minute_option;
  j2735MinuteOfTheYear minute;
  BOOL second_option;
  j2735DSecond second;
  BOOL duration_option;
  j2735DSecond duration;
  BOOL regional_option;
  j2735SignalRequestPackage_1 regional;
} j2735SignalRequestPackage;


extern const ASN1CType asn1_type_j2735SignalRequestPackage[];

typedef struct j2735SignalRequestList {
  j2735SignalRequestPackage *tab;
  size_t count;
} j2735SignalRequestList;

extern const ASN1CType asn1_type_j2735SignalRequestList[];

typedef enum j2735RequestSubRole {
  j2735RequestSubRole_requestSubRoleUnKnown,
  j2735RequestSubRole_requestSubRole1,
  j2735RequestSubRole_requestSubRole2,
  j2735RequestSubRole_requestSubRole3,
  j2735RequestSubRole_requestSubRole4,
  j2735RequestSubRole_requestSubRole5,
  j2735RequestSubRole_requestSubRole6,
  j2735RequestSubRole_requestSubRole7,
  j2735RequestSubRole_requestSubRole8,
  j2735RequestSubRole_requestSubRole9,
  j2735RequestSubRole_requestSubRole10,
  j2735RequestSubRole_requestSubRole11,
  j2735RequestSubRole_requestSubRole12,
  j2735RequestSubRole_requestSubRole13,
  j2735RequestSubRole_requestSubRole14,
  j2735RequestSubRole_requestSubRoleReserved,
} j2735RequestSubRole;

extern const ASN1CType asn1_type_j2735RequestSubRole[];

typedef enum j2735RequestImportanceLevel {
  j2735RequestImportanceLevel_requestImportanceLevelUnKnown,
  j2735RequestImportanceLevel_requestImportanceLevel1,
  j2735RequestImportanceLevel_requestImportanceLevel2,
  j2735RequestImportanceLevel_requestImportanceLevel3,
  j2735RequestImportanceLevel_requestImportanceLevel4,
  j2735RequestImportanceLevel_requestImportanceLevel5,
  j2735RequestImportanceLevel_requestImportanceLevel6,
  j2735RequestImportanceLevel_requestImportanceLevel7,
  j2735RequestImportanceLevel_requestImportanceLevel8,
  j2735RequestImportanceLevel_requestImportanceLevel9,
  j2735RequestImportanceLevel_requestImportanceLevel10,
  j2735RequestImportanceLevel_requestImportanceLevel11,
  j2735RequestImportanceLevel_requestImportanceLevel12,
  j2735RequestImportanceLevel_requestImportanceLevel13,
  j2735RequestImportanceLevel_requestImportanceLevel14,
  j2735RequestImportanceLevel_requestImportanceReserved,
} j2735RequestImportanceLevel;

extern const ASN1CType asn1_type_j2735RequestImportanceLevel[];

typedef struct j2735RegionalExtension_52 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_52;


extern const ASN1CType asn1_type_j2735RegionalExtension_52[];

typedef struct j2735RequestorType {
  j2735BasicVehicleRole role;
  BOOL subrole_option;
  j2735RequestSubRole subrole;
  BOOL request_option;
  j2735RequestImportanceLevel request;
  BOOL iso3883_option;
  j2735Iso3833VehicleType iso3883;
  BOOL hpmsType_option;
  j2735VehicleType hpmsType;
  BOOL regional_option;
  j2735RegionalExtension_52 regional;
} j2735RequestorType;


extern const ASN1CType asn1_type_j2735RequestorType[];

typedef struct j2735RequestorPositionVector {
  j2735Position3D position;
  BOOL heading_option;
  j2735Angle heading;
  BOOL speed_option;
  j2735TransmissionAndSpeed speed;
} j2735RequestorPositionVector;


extern const ASN1CType asn1_type_j2735RequestorPositionVector[];

typedef ASN1BitString j2735TransitVehicleStatus;

extern const ASN1CType asn1_type_j2735TransitVehicleStatus[];

typedef enum j2735TransitVehicleOccupancy {
  j2735TransitVehicleOccupancy_occupancyUnknown,
  j2735TransitVehicleOccupancy_occupancyEmpty,
  j2735TransitVehicleOccupancy_occupancyVeryLow,
  j2735TransitVehicleOccupancy_occupancyLow,
  j2735TransitVehicleOccupancy_occupancyMed,
  j2735TransitVehicleOccupancy_occupancyHigh,
  j2735TransitVehicleOccupancy_occupancyNearlyFull,
  j2735TransitVehicleOccupancy_occupancyFull,
} j2735TransitVehicleOccupancy;

extern const ASN1CType asn1_type_j2735TransitVehicleOccupancy[];

typedef int j2735DeltaTime;

extern const ASN1CType asn1_type_j2735DeltaTime[];

typedef struct j2735RegionalExtension_51 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_51;


extern const ASN1CType asn1_type_j2735RegionalExtension_51[];

typedef struct j2735RequestorDescription_1 {
  j2735RegionalExtension_51 *tab;
  size_t count;
} j2735RequestorDescription_1;

extern const ASN1CType asn1_type_j2735RequestorDescription_1[];

typedef struct j2735RequestorDescription {
  j2735VehicleID id;
  BOOL type_option;
  j2735RequestorType type;
  BOOL position_option;
  j2735RequestorPositionVector position;
  BOOL name_option;
  j2735DescriptiveName name;
  BOOL routeName_option;
  j2735DescriptiveName routeName;
  BOOL transitStatus_option;
  j2735TransitVehicleStatus transitStatus;
  BOOL transitOccupancy_option;
  j2735TransitVehicleOccupancy transitOccupancy;
  BOOL transitSchedule_option;
  j2735DeltaTime transitSchedule;
  BOOL regional_option;
  j2735RequestorDescription_1 regional;
} j2735RequestorDescription;


extern const ASN1CType asn1_type_j2735RequestorDescription[];

typedef struct j2735RegionalExtension_13 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_13;


extern const ASN1CType asn1_type_j2735RegionalExtension_13[];

typedef struct j2735SignalRequestMessage_1 {
  j2735RegionalExtension_13 *tab;
  size_t count;
} j2735SignalRequestMessage_1;

extern const ASN1CType asn1_type_j2735SignalRequestMessage_1[];

typedef struct j2735SignalRequestMessage {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  j2735DSecond second;
  BOOL sequenceNumber_option;
  j2735MsgCount sequenceNumber;
  BOOL requests_option;
  j2735SignalRequestList requests;
  j2735RequestorDescription requestor;
  BOOL regional_option;
  j2735SignalRequestMessage_1 regional;
} j2735SignalRequestMessage;


extern const ASN1CType asn1_type_j2735SignalRequestMessage[];

typedef struct j2735SignalRequesterInfo {
  j2735VehicleID id;
  j2735RequestID request;
  j2735MsgCount sequenceNumber;
  BOOL role_option;
  j2735BasicVehicleRole role;
  BOOL typeData_option;
  j2735RequestorType typeData;
} j2735SignalRequesterInfo;


extern const ASN1CType asn1_type_j2735SignalRequesterInfo[];

typedef enum j2735PrioritizationResponseStatus {
  j2735PrioritizationResponseStatus_unknown,
  j2735PrioritizationResponseStatus_requested,
  j2735PrioritizationResponseStatus_processing,
  j2735PrioritizationResponseStatus_watchOtherTraffic,
  j2735PrioritizationResponseStatus_granted,
  j2735PrioritizationResponseStatus_rejected,
  j2735PrioritizationResponseStatus_maxPresence,
  j2735PrioritizationResponseStatus_reserviceLocked,
} j2735PrioritizationResponseStatus;

extern const ASN1CType asn1_type_j2735PrioritizationResponseStatus[];

typedef struct j2735RegionalExtension_57 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_57;


extern const ASN1CType asn1_type_j2735RegionalExtension_57[];

typedef struct j2735SignalStatusPackage_1 {
  j2735RegionalExtension_57 *tab;
  size_t count;
} j2735SignalStatusPackage_1;

extern const ASN1CType asn1_type_j2735SignalStatusPackage_1[];

typedef struct j2735SignalStatusPackage {
  BOOL requester_option;
  j2735SignalRequesterInfo requester;
  j2735IntersectionAccessPoint inboundOn;
  BOOL outboundOn_option;
  j2735IntersectionAccessPoint outboundOn;
  BOOL minute_option;
  j2735MinuteOfTheYear minute;
  BOOL second_option;
  j2735DSecond second;
  BOOL duration_option;
  j2735DSecond duration;
  j2735PrioritizationResponseStatus status;
  BOOL regional_option;
  j2735SignalStatusPackage_1 regional;
} j2735SignalStatusPackage;


extern const ASN1CType asn1_type_j2735SignalStatusPackage[];

typedef struct j2735SignalStatusPackageList {
  j2735SignalStatusPackage *tab;
  size_t count;
} j2735SignalStatusPackageList;

extern const ASN1CType asn1_type_j2735SignalStatusPackageList[];

typedef struct j2735RegionalExtension_58 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_58;


extern const ASN1CType asn1_type_j2735RegionalExtension_58[];

typedef struct j2735SignalStatus_1 {
  j2735RegionalExtension_58 *tab;
  size_t count;
} j2735SignalStatus_1;

extern const ASN1CType asn1_type_j2735SignalStatus_1[];

typedef struct j2735SignalStatus {
  j2735MsgCount sequenceNumber;
  j2735IntersectionReferenceID id;
  j2735SignalStatusPackageList sigStatus;
  BOOL regional_option;
  j2735SignalStatus_1 regional;
} j2735SignalStatus;


extern const ASN1CType asn1_type_j2735SignalStatus[];

typedef struct j2735SignalStatusList {
  j2735SignalStatus *tab;
  size_t count;
} j2735SignalStatusList;

extern const ASN1CType asn1_type_j2735SignalStatusList[];

typedef struct j2735RegionalExtension_14 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_14;


extern const ASN1CType asn1_type_j2735RegionalExtension_14[];

typedef struct j2735SignalStatusMessage_1 {
  j2735RegionalExtension_14 *tab;
  size_t count;
} j2735SignalStatusMessage_1;

extern const ASN1CType asn1_type_j2735SignalStatusMessage_1[];

typedef struct j2735SignalStatusMessage {
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  j2735DSecond second;
  BOOL sequenceNumber_option;
  j2735MsgCount sequenceNumber;
  j2735SignalStatusList status;
  BOOL regional_option;
  j2735SignalStatusMessage_1 regional;
} j2735SignalStatusMessage;


extern const ASN1CType asn1_type_j2735SignalStatusMessage[];

typedef ASN1String j2735UniqueMSGID;

extern const ASN1CType asn1_type_j2735UniqueMSGID[];

typedef ASN1String j2735URL_Base;

extern const ASN1CType asn1_type_j2735URL_Base[];

typedef enum j2735TravelerInfoType {
  j2735TravelerInfoType_unknown,
  j2735TravelerInfoType_advisory,
  j2735TravelerInfoType_roadSignage,
  j2735TravelerInfoType_commercialSignage,
} j2735TravelerInfoType;

extern const ASN1CType asn1_type_j2735TravelerInfoType[];

typedef enum j2735MUTCDCode {
  j2735MUTCDCode_none,
  j2735MUTCDCode_regulatory,
  j2735MUTCDCode_warning,
  j2735MUTCDCode_maintenance,
  j2735MUTCDCode_motoristService,
  j2735MUTCDCode_guide,
  j2735MUTCDCode_rec,
} j2735MUTCDCode;

extern const ASN1CType asn1_type_j2735MUTCDCode[];

typedef ASN1String j2735MsgCRC;

extern const ASN1CType asn1_type_j2735MsgCRC[];

typedef struct j2735RoadSignID {
  j2735Position3D position;
  j2735HeadingSlice viewAngle;
  BOOL mutcdCode_option;
  j2735MUTCDCode mutcdCode;
  BOOL crc_option;
  j2735MsgCRC crc;
} j2735RoadSignID;


extern const ASN1CType asn1_type_j2735RoadSignID[];

typedef enum {
  j2735TravelerDataFrame_1_furtherInfoID,
  j2735TravelerDataFrame_1_roadSignID,
} j2735TravelerDataFrame_1_choice;

typedef struct j2735TravelerDataFrame_1 {
  j2735TravelerDataFrame_1_choice choice;
  union {
    j2735FurtherInfoID furtherInfoID;
    j2735RoadSignID roadSignID;
  } u;
} j2735TravelerDataFrame_1;

extern const ASN1CType asn1_type_j2735TravelerDataFrame_1[];

typedef int j2735MinutesDuration;

extern const ASN1CType asn1_type_j2735MinutesDuration[];

typedef int j2735SignPrority;

extern const ASN1CType asn1_type_j2735SignPrority[];

typedef enum j2735DirectionOfUse {
  j2735DirectionOfUse_unavailable,
  j2735DirectionOfUse_forward,
  j2735DirectionOfUse_reverse,
  j2735DirectionOfUse_both,
} j2735DirectionOfUse;

extern const ASN1CType asn1_type_j2735DirectionOfUse[];

typedef int j2735Zoom;

extern const ASN1CType asn1_type_j2735Zoom[];

typedef int j2735OffsetLL_B12;

extern const ASN1CType asn1_type_j2735OffsetLL_B12[];

typedef struct j2735Node_LL_24B {
  j2735OffsetLL_B12 lon;
  j2735OffsetLL_B12 lat;
} j2735Node_LL_24B;


extern const ASN1CType asn1_type_j2735Node_LL_24B[];

typedef int j2735OffsetLL_B14;

extern const ASN1CType asn1_type_j2735OffsetLL_B14[];

typedef struct j2735Node_LL_28B {
  j2735OffsetLL_B14 lon;
  j2735OffsetLL_B14 lat;
} j2735Node_LL_28B;


extern const ASN1CType asn1_type_j2735Node_LL_28B[];

typedef int j2735OffsetLL_B16;

extern const ASN1CType asn1_type_j2735OffsetLL_B16[];

typedef struct j2735Node_LL_32B {
  j2735OffsetLL_B16 lon;
  j2735OffsetLL_B16 lat;
} j2735Node_LL_32B;


extern const ASN1CType asn1_type_j2735Node_LL_32B[];

typedef struct j2735Node_LL_36B {
  j2735OffsetLL_B18 lon;
  j2735OffsetLL_B18 lat;
} j2735Node_LL_36B;


extern const ASN1CType asn1_type_j2735Node_LL_36B[];

typedef int j2735OffsetLL_B22;

extern const ASN1CType asn1_type_j2735OffsetLL_B22[];

typedef struct j2735Node_LL_44B {
  j2735OffsetLL_B22 lon;
  j2735OffsetLL_B22 lat;
} j2735Node_LL_44B;


extern const ASN1CType asn1_type_j2735Node_LL_44B[];

typedef int j2735OffsetLL_B24;

extern const ASN1CType asn1_type_j2735OffsetLL_B24[];

typedef struct j2735Node_LL_48B {
  j2735OffsetLL_B24 lon;
  j2735OffsetLL_B24 lat;
} j2735Node_LL_48B;


extern const ASN1CType asn1_type_j2735Node_LL_48B[];

typedef struct j2735RegionalExtension_47 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_47;


extern const ASN1CType asn1_type_j2735RegionalExtension_47[];

typedef enum {
  j2735NodeOffsetPointLL_node_LL1,
  j2735NodeOffsetPointLL_node_LL2,
  j2735NodeOffsetPointLL_node_LL3,
  j2735NodeOffsetPointLL_node_LL4,
  j2735NodeOffsetPointLL_node_LL5,
  j2735NodeOffsetPointLL_node_LL6,
  j2735NodeOffsetPointLL_node_LatLon,
  j2735NodeOffsetPointLL_regional,
} j2735NodeOffsetPointLL_choice;

typedef struct j2735NodeOffsetPointLL {
  j2735NodeOffsetPointLL_choice choice;
  union {
    j2735Node_LL_24B node_LL1;
    j2735Node_LL_28B node_LL2;
    j2735Node_LL_32B node_LL3;
    j2735Node_LL_36B node_LL4;
    j2735Node_LL_44B node_LL5;
    j2735Node_LL_48B node_LL6;
    j2735Node_LLmD_64b node_LatLon;
    j2735RegionalExtension_47 regional;
  } u;
} j2735NodeOffsetPointLL;

extern const ASN1CType asn1_type_j2735NodeOffsetPointLL[];

typedef enum j2735NodeAttributeLL {
  j2735NodeAttributeLL_reserved,
  j2735NodeAttributeLL_stopLine,
  j2735NodeAttributeLL_roundedCapStyleA,
  j2735NodeAttributeLL_roundedCapStyleB,
  j2735NodeAttributeLL_mergePoint,
  j2735NodeAttributeLL_divergePoint,
  j2735NodeAttributeLL_downstreamStopLine,
  j2735NodeAttributeLL_downstreamStartNode,
  j2735NodeAttributeLL_closedToTraffic,
  j2735NodeAttributeLL_safeIsland,
  j2735NodeAttributeLL_curbPresentAtStepOff,
  j2735NodeAttributeLL_hydrantPresent,
} j2735NodeAttributeLL;

extern const ASN1CType asn1_type_j2735NodeAttributeLL[];

typedef struct j2735NodeAttributeLLList {
  j2735NodeAttributeLL *tab;
  size_t count;
} j2735NodeAttributeLLList;

extern const ASN1CType asn1_type_j2735NodeAttributeLLList[];

typedef enum j2735SegmentAttributeLL {
  j2735SegmentAttributeLL_reserved,
  j2735SegmentAttributeLL_doNotBlock,
  j2735SegmentAttributeLL_whiteLine,
  j2735SegmentAttributeLL_mergingLaneLeft,
  j2735SegmentAttributeLL_mergingLaneRight,
  j2735SegmentAttributeLL_curbOnLeft,
  j2735SegmentAttributeLL_curbOnRight,
  j2735SegmentAttributeLL_loadingzoneOnLeft,
  j2735SegmentAttributeLL_loadingzoneOnRight,
  j2735SegmentAttributeLL_turnOutPointOnLeft,
  j2735SegmentAttributeLL_turnOutPointOnRight,
  j2735SegmentAttributeLL_adjacentParkingOnLeft,
  j2735SegmentAttributeLL_adjacentParkingOnRight,
  j2735SegmentAttributeLL_adjacentBikeLaneOnLeft,
  j2735SegmentAttributeLL_adjacentBikeLaneOnRight,
  j2735SegmentAttributeLL_sharedBikeLane,
  j2735SegmentAttributeLL_bikeBoxInFront,
  j2735SegmentAttributeLL_transitStopOnLeft,
  j2735SegmentAttributeLL_transitStopOnRight,
  j2735SegmentAttributeLL_transitStopInLane,
  j2735SegmentAttributeLL_sharedWithTrackedVehicle,
  j2735SegmentAttributeLL_safeIsland,
  j2735SegmentAttributeLL_lowCurbsPresent,
  j2735SegmentAttributeLL_rumbleStripPresent,
  j2735SegmentAttributeLL_audibleSignalingPresent,
  j2735SegmentAttributeLL_adaptiveTimingPresent,
  j2735SegmentAttributeLL_rfSignalRequestPresent,
  j2735SegmentAttributeLL_partialCurbIntrusion,
  j2735SegmentAttributeLL_taperToLeft,
  j2735SegmentAttributeLL_taperToRight,
  j2735SegmentAttributeLL_taperToCenterLine,
  j2735SegmentAttributeLL_parallelParking,
  j2735SegmentAttributeLL_headInParking,
  j2735SegmentAttributeLL_freeParking,
  j2735SegmentAttributeLL_timeRestrictionsOnParking,
  j2735SegmentAttributeLL_costToPark,
  j2735SegmentAttributeLL_midBlockCurbPresent,
  j2735SegmentAttributeLL_unEvenPavementPresent,
} j2735SegmentAttributeLL;

extern const ASN1CType asn1_type_j2735SegmentAttributeLL[];

typedef struct j2735SegmentAttributeLLList {
  j2735SegmentAttributeLL *tab;
  size_t count;
} j2735SegmentAttributeLLList;

extern const ASN1CType asn1_type_j2735SegmentAttributeLLList[];

typedef struct j2735RegionalExtension_45 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_45;


extern const ASN1CType asn1_type_j2735RegionalExtension_45[];

typedef struct j2735NodeAttributeSetLL_1 {
  j2735RegionalExtension_45 *tab;
  size_t count;
} j2735NodeAttributeSetLL_1;

extern const ASN1CType asn1_type_j2735NodeAttributeSetLL_1[];

typedef struct j2735NodeAttributeSetLL {
  BOOL localNode_option;
  j2735NodeAttributeLLList localNode;
  BOOL disabled_option;
  j2735SegmentAttributeLLList disabled;
  BOOL enabled_option;
  j2735SegmentAttributeLLList enabled;
  BOOL data_option;
  j2735LaneDataAttributeList data;
  BOOL dWidth_option;
  j2735Offset_B10 dWidth;
  BOOL dElevation_option;
  j2735Offset_B10 dElevation;
  BOOL regional_option;
  j2735NodeAttributeSetLL_1 regional;
} j2735NodeAttributeSetLL;


extern const ASN1CType asn1_type_j2735NodeAttributeSetLL[];

typedef struct j2735NodeLL {
  j2735NodeOffsetPointLL delta;
  BOOL attributes_option;
  j2735NodeAttributeSetLL attributes;
} j2735NodeLL;


extern const ASN1CType asn1_type_j2735NodeLL[];

typedef struct j2735NodeSetLL {
  j2735NodeLL *tab;
  size_t count;
} j2735NodeSetLL;

extern const ASN1CType asn1_type_j2735NodeSetLL[];

typedef enum {
  j2735NodeListLL_nodes,
} j2735NodeListLL_choice;

typedef struct j2735NodeListLL {
  j2735NodeListLL_choice choice;
  union {
    j2735NodeSetLL nodes;
  } u;
} j2735NodeListLL;

extern const ASN1CType asn1_type_j2735NodeListLL[];

typedef enum {
  j2735OffsetSystem_1_xy,
  j2735OffsetSystem_1_ll,
} j2735OffsetSystem_1_choice;

typedef struct j2735OffsetSystem_1 {
  j2735OffsetSystem_1_choice choice;
  union {
    j2735NodeListXY xy;
    j2735NodeListLL ll;
  } u;
} j2735OffsetSystem_1;

extern const ASN1CType asn1_type_j2735OffsetSystem_1[];

typedef struct j2735OffsetSystem {
  BOOL scale_option;
  j2735Zoom scale;
  j2735OffsetSystem_1 offset;
} j2735OffsetSystem;


extern const ASN1CType asn1_type_j2735OffsetSystem[];

typedef int j2735Radius_B12;

extern const ASN1CType asn1_type_j2735Radius_B12[];

typedef enum j2735DistanceUnits {
  j2735DistanceUnits_centimeter,
  j2735DistanceUnits_cm2_5,
  j2735DistanceUnits_decimeter,
  j2735DistanceUnits_meter,
  j2735DistanceUnits_kilometer,
  j2735DistanceUnits_foot,
  j2735DistanceUnits_yard,
  j2735DistanceUnits_mile,
} j2735DistanceUnits;

extern const ASN1CType asn1_type_j2735DistanceUnits[];

typedef struct j2735Circle {
  j2735Position3D center;
  j2735Radius_B12 radius;
  j2735DistanceUnits units;
} j2735Circle;


extern const ASN1CType asn1_type_j2735Circle[];

typedef struct j2735RegionalExtension_38 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_38;


extern const ASN1CType asn1_type_j2735RegionalExtension_38[];

typedef struct j2735GeometricProjection_1 {
  j2735RegionalExtension_38 *tab;
  size_t count;
} j2735GeometricProjection_1;

extern const ASN1CType asn1_type_j2735GeometricProjection_1[];

typedef struct j2735GeometricProjection {
  j2735HeadingSlice direction;
  BOOL extent_option;
  j2735Extent extent;
  BOOL laneWidth_option;
  j2735LaneWidth laneWidth;
  j2735Circle circle;
  BOOL regional_option;
  j2735GeometricProjection_1 regional;
} j2735GeometricProjection;


extern const ASN1CType asn1_type_j2735GeometricProjection[];

typedef struct j2735ShapePointSet {
  BOOL anchor_option;
  j2735Position3D anchor;
  BOOL laneWidth_option;
  j2735LaneWidth laneWidth;
  BOOL directionality_option;
  j2735DirectionOfUse directionality;
  j2735NodeListXY nodeList;
} j2735ShapePointSet;


extern const ASN1CType asn1_type_j2735ShapePointSet[];

typedef struct j2735RegionOffsets {
  j2735OffsetLL_B16 xOffset;
  j2735OffsetLL_B16 yOffset;
  BOOL zOffset_option;
  j2735OffsetLL_B16 zOffset;
} j2735RegionOffsets;


extern const ASN1CType asn1_type_j2735RegionOffsets[];

typedef struct j2735RegionList {
  j2735RegionOffsets *tab;
  size_t count;
} j2735RegionList;

extern const ASN1CType asn1_type_j2735RegionList[];

typedef struct j2735RegionPointSet {
  BOOL anchor_option;
  j2735Position3D anchor;
  BOOL scale_option;
  j2735Zoom scale;
  j2735RegionList nodeList;
} j2735RegionPointSet;


extern const ASN1CType asn1_type_j2735RegionPointSet[];

typedef enum {
  j2735ValidRegion_1_shapePointSet,
  j2735ValidRegion_1_circle,
  j2735ValidRegion_1_regionPointSet,
} j2735ValidRegion_1_choice;

typedef struct j2735ValidRegion_1 {
  j2735ValidRegion_1_choice choice;
  union {
    j2735ShapePointSet shapePointSet;
    j2735Circle circle;
    j2735RegionPointSet regionPointSet;
  } u;
} j2735ValidRegion_1;

extern const ASN1CType asn1_type_j2735ValidRegion_1[];

typedef struct j2735ValidRegion {
  j2735HeadingSlice direction;
  BOOL extent_option;
  j2735Extent extent;
  j2735ValidRegion_1 area;
} j2735ValidRegion;


extern const ASN1CType asn1_type_j2735ValidRegion[];

typedef enum {
  j2735GeographicalPath_1_path,
  j2735GeographicalPath_1_geometry,
  j2735GeographicalPath_1_oldRegion,
} j2735GeographicalPath_1_choice;

typedef struct j2735GeographicalPath_1 {
  j2735GeographicalPath_1_choice choice;
  union {
    j2735OffsetSystem path;
    j2735GeometricProjection geometry;
    j2735ValidRegion oldRegion;
  } u;
} j2735GeographicalPath_1;

extern const ASN1CType asn1_type_j2735GeographicalPath_1[];

typedef struct j2735RegionalExtension_37 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_37;


extern const ASN1CType asn1_type_j2735RegionalExtension_37[];

typedef struct j2735GeographicalPath_2 {
  j2735RegionalExtension_37 *tab;
  size_t count;
} j2735GeographicalPath_2;

extern const ASN1CType asn1_type_j2735GeographicalPath_2[];

typedef struct j2735GeographicalPath {
  BOOL name_option;
  j2735DescriptiveName name;
  BOOL id_option;
  j2735RoadSegmentReferenceID id;
  BOOL anchor_option;
  j2735Position3D anchor;
  BOOL laneWidth_option;
  j2735LaneWidth laneWidth;
  BOOL directionality_option;
  j2735DirectionOfUse directionality;
  BOOL closedPath_option;
  BOOL closedPath;
  BOOL direction_option;
  j2735HeadingSlice direction;
  BOOL description_option;
  j2735GeographicalPath_1 description;
  BOOL regional_option;
  j2735GeographicalPath_2 regional;
} j2735GeographicalPath;


extern const ASN1CType asn1_type_j2735GeographicalPath[];

typedef struct j2735TravelerDataFrame_2 {
  j2735GeographicalPath *tab;
  size_t count;
} j2735TravelerDataFrame_2;

extern const ASN1CType asn1_type_j2735TravelerDataFrame_2[];

typedef ASN1String j2735ITIStext;

extern const ASN1CType asn1_type_j2735ITIStext[];

typedef enum {
  j2735ITIScodesAndText_1_itis,
  j2735ITIScodesAndText_1_text,
} j2735ITIScodesAndText_1_choice;

typedef struct j2735ITIScodesAndText_1 {
  j2735ITIScodesAndText_1_choice choice;
  union {
    j2735ITIScodes itis;
    j2735ITIStext text;
  } u;
} j2735ITIScodesAndText_1;

extern const ASN1CType asn1_type_j2735ITIScodesAndText_1[];

typedef struct j2735ITIScodesAndText_2 {
  j2735ITIScodesAndText_1 item;
} j2735ITIScodesAndText_2;


extern const ASN1CType asn1_type_j2735ITIScodesAndText_2[];

typedef struct j2735ITIScodesAndText {
  j2735ITIScodesAndText_2 *tab;
  size_t count;
} j2735ITIScodesAndText;

extern const ASN1CType asn1_type_j2735ITIScodesAndText[];

typedef ASN1String j2735ITIStextPhrase;

extern const ASN1CType asn1_type_j2735ITIStextPhrase[];

typedef enum {
  j2735WorkZone_1_itis,
  j2735WorkZone_1_text,
} j2735WorkZone_1_choice;

typedef struct j2735WorkZone_1 {
  j2735WorkZone_1_choice choice;
  union {
    j2735ITIScodes itis;
    j2735ITIStextPhrase text;
  } u;
} j2735WorkZone_1;

extern const ASN1CType asn1_type_j2735WorkZone_1[];

typedef struct j2735WorkZone_2 {
  j2735WorkZone_1 item;
} j2735WorkZone_2;


extern const ASN1CType asn1_type_j2735WorkZone_2[];

typedef struct j2735WorkZone {
  j2735WorkZone_2 *tab;
  size_t count;
} j2735WorkZone;

extern const ASN1CType asn1_type_j2735WorkZone[];

typedef enum {
  j2735GenericSignage_1_itis,
  j2735GenericSignage_1_text,
} j2735GenericSignage_1_choice;

typedef struct j2735GenericSignage_1 {
  j2735GenericSignage_1_choice choice;
  union {
    j2735ITIScodes itis;
    j2735ITIStextPhrase text;
  } u;
} j2735GenericSignage_1;

extern const ASN1CType asn1_type_j2735GenericSignage_1[];

typedef struct j2735GenericSignage_2 {
  j2735GenericSignage_1 item;
} j2735GenericSignage_2;


extern const ASN1CType asn1_type_j2735GenericSignage_2[];

typedef struct j2735GenericSignage {
  j2735GenericSignage_2 *tab;
  size_t count;
} j2735GenericSignage;

extern const ASN1CType asn1_type_j2735GenericSignage[];

typedef enum {
  j2735SpeedLimit_1_itis,
  j2735SpeedLimit_1_text,
} j2735SpeedLimit_1_choice;

typedef struct j2735SpeedLimit_1 {
  j2735SpeedLimit_1_choice choice;
  union {
    j2735ITIScodes itis;
    j2735ITIStextPhrase text;
  } u;
} j2735SpeedLimit_1;

extern const ASN1CType asn1_type_j2735SpeedLimit_1[];

typedef struct j2735SpeedLimit_2 {
  j2735SpeedLimit_1 item;
} j2735SpeedLimit_2;


extern const ASN1CType asn1_type_j2735SpeedLimit_2[];

typedef struct j2735SpeedLimit {
  j2735SpeedLimit_2 *tab;
  size_t count;
} j2735SpeedLimit;

extern const ASN1CType asn1_type_j2735SpeedLimit[];

typedef enum {
  j2735ExitService_1_itis,
  j2735ExitService_1_text,
} j2735ExitService_1_choice;

typedef struct j2735ExitService_1 {
  j2735ExitService_1_choice choice;
  union {
    j2735ITIScodes itis;
    j2735ITIStextPhrase text;
  } u;
} j2735ExitService_1;

extern const ASN1CType asn1_type_j2735ExitService_1[];

typedef struct j2735ExitService_2 {
  j2735ExitService_1 item;
} j2735ExitService_2;


extern const ASN1CType asn1_type_j2735ExitService_2[];

typedef struct j2735ExitService {
  j2735ExitService_2 *tab;
  size_t count;
} j2735ExitService;

extern const ASN1CType asn1_type_j2735ExitService[];

typedef enum {
  j2735TravelerDataFrame_3_advisory,
  j2735TravelerDataFrame_3_workZone,
  j2735TravelerDataFrame_3_genericSign,
  j2735TravelerDataFrame_3_speedLimit,
  j2735TravelerDataFrame_3_exitService,
} j2735TravelerDataFrame_3_choice;

typedef struct j2735TravelerDataFrame_3 {
  j2735TravelerDataFrame_3_choice choice;
  union {
    j2735ITIScodesAndText advisory;
    j2735WorkZone workZone;
    j2735GenericSignage genericSign;
    j2735SpeedLimit speedLimit;
    j2735ExitService exitService;
  } u;
} j2735TravelerDataFrame_3;

extern const ASN1CType asn1_type_j2735TravelerDataFrame_3[];

typedef ASN1String j2735URL_Short;

extern const ASN1CType asn1_type_j2735URL_Short[];

typedef struct j2735TravelerDataFrame {
  j2735SSPindex sspTimRights;
  j2735TravelerInfoType frameType;
  j2735TravelerDataFrame_1 msgId;
  BOOL startYear_option;
  j2735DYear startYear;
  j2735MinuteOfTheYear startTime;
  j2735MinutesDuration duratonTime;
  j2735SignPrority priority;
  j2735SSPindex sspLocationRights;
  j2735TravelerDataFrame_2 regions;
  j2735SSPindex sspMsgRights1;
  j2735SSPindex sspMsgRights2;
  j2735TravelerDataFrame_3 content;
  BOOL url_option;
  j2735URL_Short url;
} j2735TravelerDataFrame;


extern const ASN1CType asn1_type_j2735TravelerDataFrame[];

typedef struct j2735TravelerDataFrameList {
  j2735TravelerDataFrame *tab;
  size_t count;
} j2735TravelerDataFrameList;

extern const ASN1CType asn1_type_j2735TravelerDataFrameList[];

typedef struct j2735RegionalExtension_15 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_15;


extern const ASN1CType asn1_type_j2735RegionalExtension_15[];

typedef struct j2735TravelerInformation_1 {
  j2735RegionalExtension_15 *tab;
  size_t count;
} j2735TravelerInformation_1;

extern const ASN1CType asn1_type_j2735TravelerInformation_1[];

typedef struct j2735TravelerInformation {
  j2735MsgCount msgCnt;
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL packetID_option;
  j2735UniqueMSGID packetID;
  BOOL urlB_option;
  j2735URL_Base urlB;
  j2735TravelerDataFrameList dataFrames;
  BOOL regional_option;
  j2735TravelerInformation_1 regional;
} j2735TravelerInformation;


extern const ASN1CType asn1_type_j2735TravelerInformation[];

typedef enum j2735PersonalDeviceUserType {
  j2735PersonalDeviceUserType_unavailable,
  j2735PersonalDeviceUserType_aPEDESTRIAN,
  j2735PersonalDeviceUserType_aPEDALCYCLIST,
  j2735PersonalDeviceUserType_aPUBLICSAFETYWORKER,
  j2735PersonalDeviceUserType_anANIMAL,
} j2735PersonalDeviceUserType;

extern const ASN1CType asn1_type_j2735PersonalDeviceUserType[];

typedef enum j2735HumanPropelledType {
  j2735HumanPropelledType_unavailable,
  j2735HumanPropelledType_otherTypes,
  j2735HumanPropelledType_onFoot,
  j2735HumanPropelledType_skateboard,
  j2735HumanPropelledType_pushOrKickScooter,
  j2735HumanPropelledType_wheelchair,
} j2735HumanPropelledType;

extern const ASN1CType asn1_type_j2735HumanPropelledType[];

typedef enum j2735AnimalPropelledType {
  j2735AnimalPropelledType_unavailable,
  j2735AnimalPropelledType_otherTypes,
  j2735AnimalPropelledType_animalMounted,
  j2735AnimalPropelledType_animalDrawnCarriage,
} j2735AnimalPropelledType;

extern const ASN1CType asn1_type_j2735AnimalPropelledType[];

typedef enum j2735MotorizedPropelledType {
  j2735MotorizedPropelledType_unavailable,
  j2735MotorizedPropelledType_otherTypes,
  j2735MotorizedPropelledType_wheelChair,
  j2735MotorizedPropelledType_bicycle,
  j2735MotorizedPropelledType_scooter,
  j2735MotorizedPropelledType_selfBalancingDevice,
} j2735MotorizedPropelledType;

extern const ASN1CType asn1_type_j2735MotorizedPropelledType[];

typedef enum {
  j2735PropelledInformation_human,
  j2735PropelledInformation_animal,
  j2735PropelledInformation_motor,
} j2735PropelledInformation_choice;

typedef struct j2735PropelledInformation {
  j2735PropelledInformation_choice choice;
  union {
    j2735HumanPropelledType human;
    j2735AnimalPropelledType animal;
    j2735MotorizedPropelledType motor;
  } u;
} j2735PropelledInformation;

extern const ASN1CType asn1_type_j2735PropelledInformation[];

typedef ASN1BitString j2735PersonalDeviceUsageState;

extern const ASN1CType asn1_type_j2735PersonalDeviceUsageState[];

typedef BOOL j2735PersonalCrossingRequest;

extern const ASN1CType asn1_type_j2735PersonalCrossingRequest[];

typedef BOOL j2735PersonalCrossingInProgress;

extern const ASN1CType asn1_type_j2735PersonalCrossingInProgress[];

typedef enum j2735NumberOfParticipantsInCluster {
  j2735NumberOfParticipantsInCluster_unavailable,
  j2735NumberOfParticipantsInCluster_small,
  j2735NumberOfParticipantsInCluster_medium,
  j2735NumberOfParticipantsInCluster_large,
} j2735NumberOfParticipantsInCluster;

extern const ASN1CType asn1_type_j2735NumberOfParticipantsInCluster[];

typedef int j2735PersonalClusterRadius;

extern const ASN1CType asn1_type_j2735PersonalClusterRadius[];

typedef enum j2735PublicSafetyEventResponderWorkerType {
  j2735PublicSafetyEventResponderWorkerType_unavailable,
  j2735PublicSafetyEventResponderWorkerType_towOperater,
  j2735PublicSafetyEventResponderWorkerType_fireAndEMSWorker,
  j2735PublicSafetyEventResponderWorkerType_aDOTWorker,
  j2735PublicSafetyEventResponderWorkerType_lawEnforcement,
  j2735PublicSafetyEventResponderWorkerType_hazmatResponder,
  j2735PublicSafetyEventResponderWorkerType_animalControlWorker,
  j2735PublicSafetyEventResponderWorkerType_otherPersonnel,
} j2735PublicSafetyEventResponderWorkerType;

extern const ASN1CType asn1_type_j2735PublicSafetyEventResponderWorkerType[];

typedef ASN1BitString j2735PublicSafetyAndRoadWorkerActivity;

extern const ASN1CType asn1_type_j2735PublicSafetyAndRoadWorkerActivity[];

typedef ASN1BitString j2735PublicSafetyDirectingTrafficSubType;

extern const ASN1CType asn1_type_j2735PublicSafetyDirectingTrafficSubType[];

typedef ASN1BitString j2735PersonalAssistive;

extern const ASN1CType asn1_type_j2735PersonalAssistive[];

typedef ASN1BitString j2735UserSizeAndBehaviour;

extern const ASN1CType asn1_type_j2735UserSizeAndBehaviour[];

typedef enum j2735Attachment {
  j2735Attachment_unavailable,
  j2735Attachment_stroller,
  j2735Attachment_bicycleTrailer,
  j2735Attachment_cart,
  j2735Attachment_wheelchair,
  j2735Attachment_otherWalkAssistAttachments,
  j2735Attachment_pet,
} j2735Attachment;

extern const ASN1CType asn1_type_j2735Attachment[];

typedef int j2735AttachmentRadius;

extern const ASN1CType asn1_type_j2735AttachmentRadius[];

typedef enum j2735AnimalType {
  j2735AnimalType_unavailable,
  j2735AnimalType_serviceUse,
  j2735AnimalType_pet,
  j2735AnimalType_farm,
} j2735AnimalType;

extern const ASN1CType asn1_type_j2735AnimalType[];

typedef struct j2735RegionalExtension_7 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_7;


extern const ASN1CType asn1_type_j2735RegionalExtension_7[];

typedef struct j2735PersonalSafetyMessage_1 {
  j2735RegionalExtension_7 *tab;
  size_t count;
} j2735PersonalSafetyMessage_1;

extern const ASN1CType asn1_type_j2735PersonalSafetyMessage_1[];

typedef struct j2735PersonalSafetyMessage {
  j2735PersonalDeviceUserType basicType;
  j2735DSecond secMark;
  j2735MsgCount msgCnt;
  j2735TemporaryID id;
  j2735Position3D position;
  j2735PositionalAccuracy accuracy;
  j2735Velocity speed;
  j2735Heading heading;
  BOOL accelSet_option;
  j2735AccelerationSet4Way accelSet;
  BOOL pathHistory_option;
  j2735PathHistory pathHistory;
  BOOL pathPrediction_option;
  j2735PathPrediction pathPrediction;
  BOOL propulsion_option;
  j2735PropelledInformation propulsion;
  BOOL useState_option;
  j2735PersonalDeviceUsageState useState;
  BOOL crossRequest_option;
  j2735PersonalCrossingRequest crossRequest;
  BOOL crossState_option;
  j2735PersonalCrossingInProgress crossState;
  BOOL clusterSize_option;
  j2735NumberOfParticipantsInCluster clusterSize;
  BOOL clusterRadius_option;
  j2735PersonalClusterRadius clusterRadius;
  BOOL eventResponderType_option;
  j2735PublicSafetyEventResponderWorkerType eventResponderType;
  BOOL activityType_option;
  j2735PublicSafetyAndRoadWorkerActivity activityType;
  BOOL activitySubType_option;
  j2735PublicSafetyDirectingTrafficSubType activitySubType;
  BOOL assistType_option;
  j2735PersonalAssistive assistType;
  BOOL sizing_option;
  j2735UserSizeAndBehaviour sizing;
  BOOL attachment_option;
  j2735Attachment attachment;
  BOOL attachmentRadius_option;
  j2735AttachmentRadius attachmentRadius;
  BOOL animalType_option;
  j2735AnimalType animalType;
  BOOL regional_option;
  j2735PersonalSafetyMessage_1 regional;
} j2735PersonalSafetyMessage;


extern const ASN1CType asn1_type_j2735PersonalSafetyMessage[];

typedef struct j2735Header {
  BOOL year_option;
  j2735DYear year;
  BOOL timeStamp_option;
  j2735MinuteOfTheYear timeStamp;
  BOOL secMark_option;
  j2735DSecond secMark;
  BOOL msgIssueRevision_option;
  j2735MsgCount msgIssueRevision;
} j2735Header;


extern const ASN1CType asn1_type_j2735Header[];

typedef struct j2735RegionalExtension_16 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_16;


extern const ASN1CType asn1_type_j2735RegionalExtension_16[];

typedef struct j2735TestMessage00 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_16 regional;
} j2735TestMessage00;


extern const ASN1CType asn1_type_j2735TestMessage00[];

typedef struct j2735RegionalExtension_17 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_17;


extern const ASN1CType asn1_type_j2735RegionalExtension_17[];

typedef struct j2735TestMessage01 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_17 regional;
} j2735TestMessage01;


extern const ASN1CType asn1_type_j2735TestMessage01[];

typedef struct j2735RegionalExtension_18 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_18;


extern const ASN1CType asn1_type_j2735RegionalExtension_18[];

typedef struct j2735TestMessage02 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_18 regional;
} j2735TestMessage02;


extern const ASN1CType asn1_type_j2735TestMessage02[];

typedef struct j2735RegionalExtension_19 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_19;


extern const ASN1CType asn1_type_j2735RegionalExtension_19[];

typedef struct j2735TestMessage03 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_19 regional;
} j2735TestMessage03;


extern const ASN1CType asn1_type_j2735TestMessage03[];

typedef struct j2735RegionalExtension_20 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_20;


extern const ASN1CType asn1_type_j2735RegionalExtension_20[];

typedef struct j2735TestMessage04 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_20 regional;
} j2735TestMessage04;


extern const ASN1CType asn1_type_j2735TestMessage04[];

typedef struct j2735RegionalExtension_21 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_21;


extern const ASN1CType asn1_type_j2735RegionalExtension_21[];

typedef struct j2735TestMessage05 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_21 regional;
} j2735TestMessage05;


extern const ASN1CType asn1_type_j2735TestMessage05[];

typedef struct j2735RegionalExtension_22 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_22;


extern const ASN1CType asn1_type_j2735RegionalExtension_22[];

typedef struct j2735TestMessage06 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_22 regional;
} j2735TestMessage06;


extern const ASN1CType asn1_type_j2735TestMessage06[];

typedef struct j2735RegionalExtension_23 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_23;


extern const ASN1CType asn1_type_j2735RegionalExtension_23[];

typedef struct j2735TestMessage07 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_23 regional;
} j2735TestMessage07;


extern const ASN1CType asn1_type_j2735TestMessage07[];

typedef struct j2735RegionalExtension_24 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_24;


extern const ASN1CType asn1_type_j2735RegionalExtension_24[];

typedef struct j2735TestMessage08 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_24 regional;
} j2735TestMessage08;


extern const ASN1CType asn1_type_j2735TestMessage08[];

typedef struct j2735RegionalExtension_25 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_25;


extern const ASN1CType asn1_type_j2735RegionalExtension_25[];

typedef struct j2735TestMessage09 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_25 regional;
} j2735TestMessage09;


extern const ASN1CType asn1_type_j2735TestMessage09[];

typedef struct j2735RegionalExtension_26 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_26;


extern const ASN1CType asn1_type_j2735RegionalExtension_26[];

typedef struct j2735TestMessage10 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_26 regional;
} j2735TestMessage10;


extern const ASN1CType asn1_type_j2735TestMessage10[];

typedef struct j2735RegionalExtension_27 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_27;


extern const ASN1CType asn1_type_j2735RegionalExtension_27[];

typedef struct j2735TestMessage11 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_27 regional;
} j2735TestMessage11;


extern const ASN1CType asn1_type_j2735TestMessage11[];

typedef struct j2735RegionalExtension_28 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_28;


extern const ASN1CType asn1_type_j2735RegionalExtension_28[];

typedef struct j2735TestMessage12 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_28 regional;
} j2735TestMessage12;


extern const ASN1CType asn1_type_j2735TestMessage12[];

typedef struct j2735RegionalExtension_29 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_29;


extern const ASN1CType asn1_type_j2735RegionalExtension_29[];

typedef struct j2735TestMessage13 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_29 regional;
} j2735TestMessage13;


extern const ASN1CType asn1_type_j2735TestMessage13[];

typedef struct j2735RegionalExtension_30 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_30;


extern const ASN1CType asn1_type_j2735RegionalExtension_30[];

typedef struct j2735TestMessage14 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_30 regional;
} j2735TestMessage14;


extern const ASN1CType asn1_type_j2735TestMessage14[];

typedef struct j2735RegionalExtension_31 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_31;


extern const ASN1CType asn1_type_j2735RegionalExtension_31[];

typedef struct j2735TestMessage15 {
  BOOL header_option;
  j2735Header header;
  BOOL regional_option;
  j2735RegionalExtension_31 regional;
} j2735TestMessage15;


extern const ASN1CType asn1_type_j2735TestMessage15[];

typedef struct j2735REG_EXT_ID_AND_TYPE { /* object class definition */
  ASN1CType id;
  ASN1CType Type;
} j2735REG_EXT_ID_AND_TYPE;


extern const ASN1CType asn1_type_j2735REG_EXT_ID_AND_TYPE[];

typedef struct j2735PARTII_EXT_ID_AND_TYPE { /* object class definition */
  ASN1CType id;
  ASN1CType Type;
} j2735PARTII_EXT_ID_AND_TYPE;


extern const ASN1CType asn1_type_j2735PARTII_EXT_ID_AND_TYPE[];

typedef struct j2735EventDescription_1 {
  j2735ITIScodes *tab;
  size_t count;
} j2735EventDescription_1;

extern const ASN1CType asn1_type_j2735EventDescription_1[];

typedef struct j2735RegionalExtension_35 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_35;


extern const ASN1CType asn1_type_j2735RegionalExtension_35[];

typedef struct j2735EventDescription_2 {
  j2735RegionalExtension_35 *tab;
  size_t count;
} j2735EventDescription_2;

extern const ASN1CType asn1_type_j2735EventDescription_2[];

typedef struct j2735EventDescription {
  j2735ITIScodes typeEvent;
  BOOL description_option;
  j2735EventDescription_1 description;
  BOOL priority_option;
  j2735Priority priority;
  BOOL heading_option;
  j2735HeadingSlice heading;
  BOOL extent_option;
  j2735Extent extent;
  BOOL regional_option;
  j2735EventDescription_2 regional;
} j2735EventDescription;


extern const ASN1CType asn1_type_j2735EventDescription[];

typedef BOOL j2735PivotingAllowed;

extern const ASN1CType asn1_type_j2735PivotingAllowed[];

typedef struct j2735PivotPointDescription {
  j2735Offset_B11 pivotOffset;
  j2735Angle pivotAngle;
  j2735PivotingAllowed pivots;
} j2735PivotPointDescription;


extern const ASN1CType asn1_type_j2735PivotPointDescription[];

typedef BOOL j2735IsDolly;

extern const ASN1CType asn1_type_j2735IsDolly[];

typedef int j2735TrailerMass;

extern const ASN1CType asn1_type_j2735TrailerMass[];

typedef int j2735VertOffset_B07;

extern const ASN1CType asn1_type_j2735VertOffset_B07[];

typedef struct j2735TrailerHistoryPoint {
  j2735Angle pivotAngle;
  j2735TimeOffset timeOffset;
  j2735Node_XY_24b positionOffset;
  BOOL elevationOffset_option;
  j2735VertOffset_B07 elevationOffset;
  BOOL heading_option;
  j2735CoarseHeading heading;
} j2735TrailerHistoryPoint;


extern const ASN1CType asn1_type_j2735TrailerHistoryPoint[];

typedef struct j2735TrailerHistoryPointList {
  j2735TrailerHistoryPoint *tab;
  size_t count;
} j2735TrailerHistoryPointList;

extern const ASN1CType asn1_type_j2735TrailerHistoryPointList[];

typedef struct j2735TrailerUnitDescription {
  j2735IsDolly isDolly;
  j2735VehicleWidth width;
  j2735VehicleLength length;
  BOOL height_option;
  j2735VehicleHeight height;
  BOOL mass_option;
  j2735TrailerMass mass;
  BOOL bumperHeights_option;
  j2735BumperHeights bumperHeights;
  BOOL centerOfGravity_option;
  j2735VehicleHeight centerOfGravity;
  j2735PivotPointDescription frontPivot;
  BOOL rearPivot_option;
  j2735PivotPointDescription rearPivot;
  BOOL rearWheelOffset_option;
  j2735Offset_B12 rearWheelOffset;
  j2735Node_XY_24b positionOffset;
  BOOL elevationOffset_option;
  j2735VertOffset_B07 elevationOffset;
  BOOL crumbData_option;
  j2735TrailerHistoryPointList crumbData;
} j2735TrailerUnitDescription;


extern const ASN1CType asn1_type_j2735TrailerUnitDescription[];

typedef struct j2735TrailerUnitDescriptionList {
  j2735TrailerUnitDescription *tab;
  size_t count;
} j2735TrailerUnitDescriptionList;

extern const ASN1CType asn1_type_j2735TrailerUnitDescriptionList[];

typedef struct j2735TrailerData {
  j2735SSPindex sspRights;
  j2735PivotPointDescription connection;
  j2735TrailerUnitDescriptionList units;
} j2735TrailerData;


extern const ASN1CType asn1_type_j2735TrailerData[];

typedef struct j2735SpecialVehicleExtensions {
  BOOL vehicleAlerts_option;
  j2735EmergencyDetails vehicleAlerts;
  BOOL description_option;
  j2735EventDescription description;
  BOOL trailers_option;
  j2735TrailerData trailers;
} j2735SpecialVehicleExtensions;


extern const ASN1CType asn1_type_j2735SpecialVehicleExtensions[];

typedef struct j2735VehicleData {
  BOOL height_option;
  j2735VehicleHeight height;
  BOOL bumpers_option;
  j2735BumperHeights bumpers;
  BOOL mass_option;
  j2735VehicleMass mass;
  BOOL trailerWeight_option;
  j2735TrailerWeight trailerWeight;
} j2735VehicleData;


extern const ASN1CType asn1_type_j2735VehicleData[];

typedef struct j2735WeatherReport {
  j2735EssPrecipYesNo isRaining;
  BOOL rainRate_option;
  j2735EssPrecipRate rainRate;
  BOOL precipSituation_option;
  j2735EssPrecipSituation precipSituation;
  BOOL solarRadiation_option;
  j2735EssSolarRadiation solarRadiation;
  BOOL friction_option;
  j2735EssMobileFriction friction;
  BOOL roadFriction_option;
  j2735CoefficientOfFriction roadFriction;
} j2735WeatherReport;


extern const ASN1CType asn1_type_j2735WeatherReport[];

typedef struct j2735WeatherProbe {
  BOOL airTemp_option;
  j2735AmbientAirTemperature airTemp;
  BOOL airPressure_option;
  j2735AmbientAirPressure airPressure;
  BOOL rainRates_option;
  j2735WiperSet rainRates;
} j2735WeatherProbe;


extern const ASN1CType asn1_type_j2735WeatherProbe[];

typedef j2735Angle j2735ObstacleDirection;

extern const ASN1CType asn1_type_j2735ObstacleDirection[];

typedef int j2735ITIScodes_2;

extern const ASN1CType asn1_type_j2735ITIScodes_2[];

typedef enum j2735GenericLocations {
  j2735GenericLocations_on_bridges,
  j2735GenericLocations_in_tunnels,
  j2735GenericLocations_entering_or_leaving_tunnels,
  j2735GenericLocations_on_ramps,
  j2735GenericLocations_in_road_construction_area,
  j2735GenericLocations_around_a_curve,
  j2735GenericLocations_on_curve,
  j2735GenericLocations_on_tracks,
  j2735GenericLocations_in_street,
  j2735GenericLocations_shoulder,
  j2735GenericLocations_on_minor_roads,
  j2735GenericLocations_in_the_opposing_lanes,
  j2735GenericLocations_adjacent_to_roadway,
  j2735GenericLocations_across_tracks,
  j2735GenericLocations_on_bend,
  j2735GenericLocations_intersection,
  j2735GenericLocations_entire_intersection,
  j2735GenericLocations_in_the_median,
  j2735GenericLocations_moved_to_side_of_road,
  j2735GenericLocations_moved_to_shoulder,
  j2735GenericLocations_on_the_roadway,
  j2735GenericLocations_dip,
  j2735GenericLocations_traffic_circle,
  j2735GenericLocations_crossover,
  j2735GenericLocations_cross_road,
  j2735GenericLocations_side_road,
  j2735GenericLocations_to,
  j2735GenericLocations_by,
  j2735GenericLocations_through,
  j2735GenericLocations_area_of,
  j2735GenericLocations_under,
  j2735GenericLocations_over,
  j2735GenericLocations_from,
  j2735GenericLocations_approaching,
  j2735GenericLocations_entering_at,
  j2735GenericLocations_exiting_at,
  j2735GenericLocations_in_shaded_areas,
  j2735GenericLocations_in_low_lying_areas,
  j2735GenericLocations_in_the_downtown_area,
  j2735GenericLocations_in_the_inner_city_area,
  j2735GenericLocations_in_parts,
  j2735GenericLocations_in_some_places,
  j2735GenericLocations_in_the_ditch,
  j2735GenericLocations_in_the_valley,
  j2735GenericLocations_on_hill_top,
  j2735GenericLocations_near_the_foothills,
  j2735GenericLocations_at_high_altitudes,
  j2735GenericLocations_near_the_lake,
  j2735GenericLocations_near_the_shore,
  j2735GenericLocations_nearby_basin,
  j2735GenericLocations_over_the_crest_of_a_hill,
  j2735GenericLocations_other_than_on_the_roadway,
  j2735GenericLocations_near_the_beach,
  j2735GenericLocations_near_beach_access_point,
  j2735GenericLocations_mountain_pass,
  j2735GenericLocations_lower_level,
  j2735GenericLocations_upper_level,
  j2735GenericLocations_airport,
  j2735GenericLocations_concourse,
  j2735GenericLocations_gate,
  j2735GenericLocations_baggage_claim,
  j2735GenericLocations_customs_point,
  j2735GenericLocations_reservation_center,
  j2735GenericLocations_station,
  j2735GenericLocations_platform,
  j2735GenericLocations_dock,
  j2735GenericLocations_depot,
  j2735GenericLocations_ev_charging_point,
  j2735GenericLocations_information_welcome_point,
  j2735GenericLocations_at_rest_area,
  j2735GenericLocations_at_service_area,
  j2735GenericLocations_at_weigh_station,
  j2735GenericLocations_roadside_park,
  j2735GenericLocations_picnic_areas,
  j2735GenericLocations_rest_area,
  j2735GenericLocations_service_stations,
  j2735GenericLocations_toilets,
  j2735GenericLocations_bus_stop,
  j2735GenericLocations_park_and_ride_lot,
  j2735GenericLocations_on_the_right,
  j2735GenericLocations_on_the_left,
  j2735GenericLocations_in_the_center,
  j2735GenericLocations_in_the_opposite_direction,
  j2735GenericLocations_cross_traffic,
  j2735GenericLocations_northbound_traffic,
  j2735GenericLocations_eastbound_traffic,
  j2735GenericLocations_southbound_traffic,
  j2735GenericLocations_westbound_traffic,
  j2735GenericLocations_north,
  j2735GenericLocations_south,
  j2735GenericLocations_east,
  j2735GenericLocations_west,
  j2735GenericLocations_northeast,
  j2735GenericLocations_northwest,
  j2735GenericLocations_southeast,
  j2735GenericLocations_southwest,
} j2735GenericLocations;

extern const ASN1CType asn1_type_j2735GenericLocations[];

typedef struct j2735ObstacleDetection {
  j2735ObstacleDistance obDist;
  j2735ObstacleDirection obDirect;
  BOOL description_option;
  j2735ITIScodes_2 description;
  BOOL locationDetails_option;
  j2735GenericLocations locationDetails;
  j2735DDateTime dateTime;
  BOOL vertEvent_option;
  j2735VerticalAccelerationThreshold vertEvent;
} j2735ObstacleDetection;


extern const ASN1CType asn1_type_j2735ObstacleDetection[];

typedef int j2735ITIScodes_1;

extern const ASN1CType asn1_type_j2735ITIScodes_1[];

typedef struct j2735DisabledVehicle {
  j2735ITIScodes_1 statusDetails;
  BOOL locationDetails_option;
  j2735GenericLocations locationDetails;
} j2735DisabledVehicle;


extern const ASN1CType asn1_type_j2735DisabledVehicle[];

typedef j2735GrossSpeed j2735SpeedProfileMeasurement;

#define asn1_type_j2735SpeedProfileMeasurement asn1_type_j2735GrossSpeed

typedef struct j2735SpeedProfileMeasurementList {
  j2735SpeedProfileMeasurement *tab;
  size_t count;
} j2735SpeedProfileMeasurementList;

extern const ASN1CType asn1_type_j2735SpeedProfileMeasurementList[];

typedef struct j2735SpeedProfile {
  j2735SpeedProfileMeasurementList speedReports;
} j2735SpeedProfile;


extern const ASN1CType asn1_type_j2735SpeedProfile[];

typedef struct j2735RTCMPackage {
  BOOL rtcmHeader_option;
  j2735RTCMheader rtcmHeader;
  j2735RTCMmessageList msgs;
} j2735RTCMPackage;


extern const ASN1CType asn1_type_j2735RTCMPackage[];

typedef struct j2735RegionalExtension_59 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_59;


extern const ASN1CType asn1_type_j2735RegionalExtension_59[];

typedef struct j2735SupplementalVehicleExtensions_1 {
  j2735RegionalExtension_59 *tab;
  size_t count;
} j2735SupplementalVehicleExtensions_1;

extern const ASN1CType asn1_type_j2735SupplementalVehicleExtensions_1[];

typedef struct j2735SupplementalVehicleExtensions {
  BOOL classification_option;
  j2735BasicVehicleClass classification;
  BOOL classDetails_option;
  j2735VehicleClassification classDetails;
  BOOL vehicleData_option;
  j2735VehicleData vehicleData;
  BOOL weatherReport_option;
  j2735WeatherReport weatherReport;
  BOOL weatherProbe_option;
  j2735WeatherProbe weatherProbe;
  BOOL obstacle_option;
  j2735ObstacleDetection obstacle;
  BOOL status_option;
  j2735DisabledVehicle status;
  BOOL speedProfile_option;
  j2735SpeedProfile speedProfile;
  BOOL theRTCM_option;
  j2735RTCMPackage theRTCM;
  BOOL regional_option;
  j2735SupplementalVehicleExtensions_1 regional;
} j2735SupplementalVehicleExtensions;


extern const ASN1CType asn1_type_j2735SupplementalVehicleExtensions[];

typedef struct j2735DDate {
  j2735DYear year;
  j2735DMonth month;
  j2735DDay day;
} j2735DDate;


extern const ASN1CType asn1_type_j2735DDate[];

typedef struct j2735DFullTime {
  j2735DYear year;
  j2735DMonth month;
  j2735DDay day;
  j2735DHour hour;
  j2735DMinute minute;
} j2735DFullTime;


extern const ASN1CType asn1_type_j2735DFullTime[];

typedef struct j2735DMonthDay {
  j2735DMonth month;
  j2735DDay day;
} j2735DMonthDay;


extern const ASN1CType asn1_type_j2735DMonthDay[];

typedef struct j2735DTime {
  j2735DHour hour;
  j2735DMinute minute;
  j2735DSecond second;
  BOOL offset_option;
  j2735DOffset offset;
} j2735DTime;


extern const ASN1CType asn1_type_j2735DTime[];

typedef struct j2735DYearMonth {
  j2735DYear year;
  j2735DMonth month;
} j2735DYearMonth;


extern const ASN1CType asn1_type_j2735DYearMonth[];

typedef int j2735VertOffset_B08;

extern const ASN1CType asn1_type_j2735VertOffset_B08[];

typedef int j2735VertOffset_B09;

extern const ASN1CType asn1_type_j2735VertOffset_B09[];

typedef int j2735VertOffset_B10;

extern const ASN1CType asn1_type_j2735VertOffset_B10[];

typedef int j2735VertOffset_B11;

extern const ASN1CType asn1_type_j2735VertOffset_B11[];

typedef struct j2735RegionalExtension_61 {
  j2735RegionId regionId;
  ASN1OpenType regExtValue;
} j2735RegionalExtension_61;


extern const ASN1CType asn1_type_j2735RegionalExtension_61[];

typedef enum {
  j2735VerticalOffset_offset1,
  j2735VerticalOffset_offset2,
  j2735VerticalOffset_offset3,
  j2735VerticalOffset_offset4,
  j2735VerticalOffset_offset5,
  j2735VerticalOffset_offset6,
  j2735VerticalOffset_elevation,
  j2735VerticalOffset_regional,
} j2735VerticalOffset_choice;

typedef struct j2735VerticalOffset {
  j2735VerticalOffset_choice choice;
  union {
    j2735VertOffset_B07 offset1;
    j2735VertOffset_B08 offset2;
    j2735VertOffset_B09 offset3;
    j2735VertOffset_B10 offset4;
    j2735VertOffset_B11 offset5;
    j2735VertOffset_B12 offset6;
    j2735Elevation elevation;
    j2735RegionalExtension_61 regional;
  } u;
} j2735VerticalOffset;

extern const ASN1CType asn1_type_j2735VerticalOffset[];

typedef ASN1String j2735CodeWord;

extern const ASN1CType asn1_type_j2735CodeWord[];

typedef int j2735Count;

extern const ASN1CType asn1_type_j2735Count[];

typedef int j2735Duration;

extern const ASN1CType asn1_type_j2735Duration[];

typedef enum j2735Location_quality {
  j2735Location_quality_loc_qual_bt1m,
  j2735Location_quality_loc_qual_bt5m,
  j2735Location_quality_loc_qual_bt12m,
  j2735Location_quality_loc_qual_bt50m,
  j2735Location_quality_loc_qual_bt125m,
  j2735Location_quality_loc_qual_bt500m,
  j2735Location_quality_loc_qual_bt1250m,
  j2735Location_quality_loc_qual_unknown,
} j2735Location_quality;

extern const ASN1CType asn1_type_j2735Location_quality[];

typedef enum j2735Location_tech {
  j2735Location_tech_loc_tech_unknown,
  j2735Location_tech_loc_tech_GNSS,
  j2735Location_tech_loc_tech_DGPS,
  j2735Location_tech_loc_tech_RTK,
  j2735Location_tech_loc_tech_PPP,
  j2735Location_tech_loc_tech_drGPS,
  j2735Location_tech_loc_tech_drDGPS,
  j2735Location_tech_loc_tech_dr,
  j2735Location_tech_loc_tech_nav,
  j2735Location_tech_loc_tech_fault,
} j2735Location_tech;

extern const ASN1CType asn1_type_j2735Location_tech[];

typedef ASN1String j2735MessageBLOB;

extern const ASN1CType asn1_type_j2735MessageBLOB[];

typedef ASN1String j2735PayloadData;

extern const ASN1CType asn1_type_j2735PayloadData[];

typedef ASN1String j2735SignalReqScheme;

extern const ASN1CType asn1_type_j2735SignalReqScheme[];

typedef ASN1BitString j2735TransitStatus;

extern const ASN1CType asn1_type_j2735TransitStatus[];

typedef ASN1String j2735URL_Link;

extern const ASN1CType asn1_type_j2735URL_Link[];

typedef struct j2735VehicleToLanePosition {
  j2735StationID stationID;
  j2735LaneID laneID;
} j2735VehicleToLanePosition;


extern const ASN1CType asn1_type_j2735VehicleToLanePosition[];

typedef struct j2735VehicleToLanePositionList {
  j2735VehicleToLanePosition *tab;
  size_t count;
} j2735VehicleToLanePositionList;

extern const ASN1CType asn1_type_j2735VehicleToLanePositionList[];

typedef struct j2735ConnectionManeuverAssist_addGrpC {
  j2735VehicleToLanePositionList vehicleToLanePositions;
  BOOL rsuDistanceFromAnchor_option;
  j2735NodeOffsetPointXY rsuDistanceFromAnchor;
} j2735ConnectionManeuverAssist_addGrpC;


extern const ASN1CType asn1_type_j2735ConnectionManeuverAssist_addGrpC[];

typedef struct j2735PrioritizationResponse {
  j2735StationID stationID;
  j2735PrioritizationResponseStatus priorState;
  j2735SignalGroupID signalGroup;
} j2735PrioritizationResponse;


extern const ASN1CType asn1_type_j2735PrioritizationResponse[];

typedef struct j2735PrioritizationResponseList {
  j2735PrioritizationResponse *tab;
  size_t count;
} j2735PrioritizationResponseList;

extern const ASN1CType asn1_type_j2735PrioritizationResponseList[];

typedef struct j2735IntersectionState_addGrpC {
  BOOL activePrioritizations_option;
  j2735PrioritizationResponseList activePrioritizations;
} j2735IntersectionState_addGrpC;


extern const ASN1CType asn1_type_j2735IntersectionState_addGrpC[];

typedef struct j2735LaneDataAttribute_addGrpB {
  uint8_t dummy_field;
} j2735LaneDataAttribute_addGrpB;


extern const ASN1CType asn1_type_j2735LaneDataAttribute_addGrpB[];

typedef int j2735TimeRemaining;

extern const ASN1CType asn1_type_j2735TimeRemaining[];

typedef int j2735MinTimetoChange;

extern const ASN1CType asn1_type_j2735MinTimetoChange[];

typedef int j2735MaxTimetoChange;

extern const ASN1CType asn1_type_j2735MaxTimetoChange[];

typedef struct j2735MovementEvent_addGrpB {
  BOOL startTime_option;
  j2735TimeRemaining startTime;
  j2735MinTimetoChange minEndTime;
  BOOL maxEndTime_option;
  j2735MaxTimetoChange maxEndTime;
  BOOL likelyTime_option;
  j2735TimeRemaining likelyTime;
  BOOL confidence_option;
  j2735TimeIntervalConfidence confidence;
  BOOL nextTime_option;
  j2735TimeRemaining nextTime;
} j2735MovementEvent_addGrpB;


extern const ASN1CType asn1_type_j2735MovementEvent_addGrpB[];

typedef int j2735LongitudeDMS;

extern const ASN1CType asn1_type_j2735LongitudeDMS[];

typedef int j2735LatitudeDMS;

extern const ASN1CType asn1_type_j2735LatitudeDMS[];

typedef struct j2735Node_LLdms_48b {
  j2735LongitudeDMS lon;
  j2735LatitudeDMS lat;
} j2735Node_LLdms_48b;


extern const ASN1CType asn1_type_j2735Node_LLdms_48b[];

typedef int j2735DegreesLong;

extern const ASN1CType asn1_type_j2735DegreesLong[];

typedef int j2735MinutesAngle;

extern const ASN1CType asn1_type_j2735MinutesAngle[];

typedef int j2735SecondsAngle;

extern const ASN1CType asn1_type_j2735SecondsAngle[];

typedef struct j2735LongitudeDMS2 {
  j2735DegreesLong d;
  j2735MinutesAngle m;
  j2735SecondsAngle s;
} j2735LongitudeDMS2;


extern const ASN1CType asn1_type_j2735LongitudeDMS2[];

typedef int j2735DegreesLat;

extern const ASN1CType asn1_type_j2735DegreesLat[];

typedef struct j2735LatitudeDMS2 {
  j2735DegreesLat d;
  j2735MinutesAngle m;
  j2735SecondsAngle s;
} j2735LatitudeDMS2;


extern const ASN1CType asn1_type_j2735LatitudeDMS2[];

typedef struct j2735Node_LLdms_80b {
  j2735LongitudeDMS2 lon;
  j2735LatitudeDMS2 lat;
} j2735Node_LLdms_80b;


extern const ASN1CType asn1_type_j2735Node_LLdms_80b[];

typedef enum {
  j2735NodeOffsetPointXY_addGrpB_posA,
  j2735NodeOffsetPointXY_addGrpB_posB,
} j2735NodeOffsetPointXY_addGrpB_choice;

typedef struct j2735NodeOffsetPointXY_addGrpB {
  j2735NodeOffsetPointXY_addGrpB_choice choice;
  union {
    j2735Node_LLdms_48b posA;
    j2735Node_LLdms_80b posB;
  } u;
} j2735NodeOffsetPointXY_addGrpB;

extern const ASN1CType asn1_type_j2735NodeOffsetPointXY_addGrpB[];

typedef int j2735AddGrpBElevation;

extern const ASN1CType asn1_type_j2735AddGrpBElevation[];

typedef struct j2735Position3D_addGrpB {
  j2735LatitudeDMS2 latitude;
  j2735LongitudeDMS2 longitude;
  j2735AddGrpBElevation elevation;
} j2735Position3D_addGrpB;


extern const ASN1CType asn1_type_j2735Position3D_addGrpB[];

typedef int j2735AltitudeValue;

extern const ASN1CType asn1_type_j2735AltitudeValue[];

typedef enum j2735AltitudeConfidence {
  j2735AltitudeConfidence_alt_000_01,
  j2735AltitudeConfidence_alt_000_02,
  j2735AltitudeConfidence_alt_000_05,
  j2735AltitudeConfidence_alt_000_10,
  j2735AltitudeConfidence_alt_000_20,
  j2735AltitudeConfidence_alt_000_50,
  j2735AltitudeConfidence_alt_001_00,
  j2735AltitudeConfidence_alt_002_00,
  j2735AltitudeConfidence_alt_005_00,
  j2735AltitudeConfidence_alt_010_00,
  j2735AltitudeConfidence_alt_020_00,
  j2735AltitudeConfidence_alt_050_00,
  j2735AltitudeConfidence_alt_100_00,
  j2735AltitudeConfidence_alt_200_00,
  j2735AltitudeConfidence_outOfRange,
  j2735AltitudeConfidence_unavailable,
} j2735AltitudeConfidence;

extern const ASN1CType asn1_type_j2735AltitudeConfidence[];

typedef struct j2735Altitude {
  j2735AltitudeValue value;
  j2735AltitudeConfidence confidence;
} j2735Altitude;


extern const ASN1CType asn1_type_j2735Altitude[];

typedef struct j2735Position3D_addGrpC {
  j2735Altitude altitude;
} j2735Position3D_addGrpC;


extern const ASN1CType asn1_type_j2735Position3D_addGrpC[];

typedef enum j2735EmissionType {
  j2735EmissionType_typeA,
  j2735EmissionType_typeB,
  j2735EmissionType_typeC,
  j2735EmissionType_typeD,
  j2735EmissionType_typeE,
} j2735EmissionType;

extern const ASN1CType asn1_type_j2735EmissionType[];

typedef struct j2735RestrictionUserType_addGrpC {
  BOOL emission_option;
  j2735EmissionType emission;
} j2735RestrictionUserType_addGrpC;


extern const ASN1CType asn1_type_j2735RestrictionUserType_addGrpC[];

typedef struct j2735SignalHeadLocation {
  j2735NodeOffsetPointXY node;
  j2735SignalGroupID signalGroupID;
} j2735SignalHeadLocation;


extern const ASN1CType asn1_type_j2735SignalHeadLocation[];

typedef struct j2735SignalHeadLocationList {
  j2735SignalHeadLocation *tab;
  size_t count;
} j2735SignalHeadLocationList;

extern const ASN1CType asn1_type_j2735SignalHeadLocationList[];

typedef struct j2735MapData_addGrpC {
  BOOL signalHeadLocations_option;
  j2735SignalHeadLocationList signalHeadLocations;
} j2735MapData_addGrpC;


extern const ASN1CType asn1_type_j2735MapData_addGrpC[];

typedef int j2735AddGrpBAngle;

extern const ASN1CType asn1_type_j2735AddGrpBAngle[];

typedef int j2735Day;

extern const ASN1CType asn1_type_j2735Day[];

typedef enum j2735DayOfWeek {
  j2735DayOfWeek_unknown,
  j2735DayOfWeek_monday,
  j2735DayOfWeek_tuesday,
  j2735DayOfWeek_wednesday,
  j2735DayOfWeek_thursday,
  j2735DayOfWeek_friday,
  j2735DayOfWeek_saturday,
  j2735DayOfWeek_sunday,
} j2735DayOfWeek;

extern const ASN1CType asn1_type_j2735DayOfWeek[];

typedef enum j2735Holiday {
  j2735Holiday_weekday,
  j2735Holiday_holiday,
} j2735Holiday;

extern const ASN1CType asn1_type_j2735Holiday[];

typedef int j2735Hour;

extern const ASN1CType asn1_type_j2735Hour[];

typedef int j2735Minute;

extern const ASN1CType asn1_type_j2735Minute[];

typedef int j2735Month;

extern const ASN1CType asn1_type_j2735Month[];

typedef int j2735AddGrpBMsgCount;

extern const ASN1CType asn1_type_j2735AddGrpBMsgCount[];

typedef int j2735Second;

extern const ASN1CType asn1_type_j2735Second[];

typedef enum j2735SummerTime {
  j2735SummerTime_notInSummerTime,
  j2735SummerTime_inSummerTime,
} j2735SummerTime;

extern const ASN1CType asn1_type_j2735SummerTime[];

typedef int j2735TenthSecond;

extern const ASN1CType asn1_type_j2735TenthSecond[];

typedef int j2735Year;

extern const ASN1CType asn1_type_j2735Year[];

typedef struct j2735AddGrpBTimeMark {
  j2735Year year;
  j2735Month month;
  j2735Day day;
  j2735SummerTime summerTime;
  j2735Holiday holiday;
  j2735DayOfWeek dayofWeek;
  j2735Hour hour;
  j2735Minute minute;
  j2735Second second;
  j2735TenthSecond tenthSecond;
} j2735AddGrpBTimeMark;


extern const ASN1CType asn1_type_j2735AddGrpBTimeMark[];

#ifdef  __cplusplus
}
#endif

#endif /* _FFASN1_FFASN1_J2735_2016_H */
