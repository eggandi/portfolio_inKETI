/* Automatically generated file - do not edit */
#ifndef _FFASN1_FFASN1_DOT3_2016_H
#define _FFASN1_FFASN1_DOT3_2016_H

#include "asn1defs.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef int dot3Ext3;

extern const ASN1CType asn1_type_dot3Ext3[];

typedef enum {
  dot3Ext2_content,
  dot3Ext2_extension,
} dot3Ext2_choice;

typedef struct dot3Ext2 {
  dot3Ext2_choice choice;
  union {
    int content;
    dot3Ext3 extension;
  } u;
} dot3Ext2;

extern const ASN1CType asn1_type_dot3Ext2[];

typedef enum {
  dot3Ext1_content,
  dot3Ext1_extension,
} dot3Ext1_choice;

typedef struct dot3Ext1 {
  dot3Ext1_choice choice;
  union {
    int content;
    dot3Ext2 extension;
  } u;
} dot3Ext1;

extern const ASN1CType asn1_type_dot3Ext1[];

typedef enum {
  dot3VarLengthNumber_content,
  dot3VarLengthNumber_extension,
} dot3VarLengthNumber_choice;

typedef struct dot3VarLengthNumber {
  dot3VarLengthNumber_choice choice;
  union {
    int content;
    dot3Ext1 extension;
  } u;
} dot3VarLengthNumber;

extern const ASN1CType asn1_type_dot3VarLengthNumber[];

typedef enum {
  dot3VarLengthNumber2_shortNo,
  dot3VarLengthNumber2_longNo,
} dot3VarLengthNumber2_choice;

typedef struct dot3VarLengthNumber2 {
  dot3VarLengthNumber2_choice choice;
  union {
    int shortNo;
    int longNo;
  } u;
} dot3VarLengthNumber2;

extern const ASN1CType asn1_type_dot3VarLengthNumber2[];

typedef int dot3RefExt;

extern const ASN1CType asn1_type_dot3RefExt[];

typedef struct dot3EXT_TYPE { /* object class definition */
  ASN1CType extRef;
  ASN1CType ExtValue;
} dot3EXT_TYPE;


extern const ASN1CType asn1_type_dot3EXT_TYPE[];

typedef int dot3DataRate80211;

extern const ASN1CType asn1_type_dot3DataRate80211[];

typedef int dot3TXpower80211;

extern const ASN1CType asn1_type_dot3TXpower80211[];

typedef int dot3ChannelNumber80211;

extern const ASN1CType asn1_type_dot3ChannelNumber80211[];

typedef int dot3RepeatRate;

extern const ASN1CType asn1_type_dot3RepeatRate[];

typedef struct dot3Latitude {
  ASN1BitString fill;
  int lat;
} dot3Latitude;


extern const ASN1CType asn1_type_dot3Latitude[];

typedef int dot3Longitude;

extern const ASN1CType asn1_type_dot3Longitude[];

typedef struct dot3TwoDLocation {
  dot3Latitude latitude;
  dot3Longitude longitude;
} dot3TwoDLocation;


extern const ASN1CType asn1_type_dot3TwoDLocation[];

typedef int dot3Elevation;

extern const ASN1CType asn1_type_dot3Elevation[];

typedef struct dot3ThreeDLocation {
  dot3Latitude latitude;
  dot3Longitude longitude;
  dot3Elevation elevation;
} dot3ThreeDLocation;


extern const ASN1CType asn1_type_dot3ThreeDLocation[];

typedef ASN1String dot3AdvertiserIdentifier;

extern const ASN1CType asn1_type_dot3AdvertiserIdentifier[];

typedef struct dot3ProviderServiceContext {
  ASN1BitString fillBit;
  ASN1String psc;
} dot3ProviderServiceContext;


extern const ASN1CType asn1_type_dot3ProviderServiceContext[];

typedef ASN1String dot3IPv6Address;

extern const ASN1CType asn1_type_dot3IPv6Address[];

typedef int dot3ServicePort;

extern const ASN1CType asn1_type_dot3ServicePort[];

typedef ASN1String dot3MACaddress;

extern const ASN1CType asn1_type_dot3MACaddress[];

typedef dot3MACaddress dot3ProviderMacAddress;

#define asn1_type_dot3ProviderMacAddress asn1_type_dot3MACaddress

typedef int dot3RcpiThreshold;

extern const ASN1CType asn1_type_dot3RcpiThreshold[];

typedef int dot3WsaCountThreshold;

extern const ASN1CType asn1_type_dot3WsaCountThreshold[];

typedef int dot3WsaCountThresholdInterval;

extern const ASN1CType asn1_type_dot3WsaCountThresholdInterval[];

typedef struct dot3EdcaParameterRecord {
  int res;
  int aci;
  int acm;
  int aifsn;
  int ecwMax;
  int ecwMin;
  int txopLimit;
} dot3EdcaParameterRecord;


extern const ASN1CType asn1_type_dot3EdcaParameterRecord[];

typedef struct dot3EdcaParameterSet {
  dot3EdcaParameterRecord acbeRecord;
  dot3EdcaParameterRecord acbkRecord;
  dot3EdcaParameterRecord acviRecord;
  dot3EdcaParameterRecord acvoRecord;
} dot3EdcaParameterSet;


extern const ASN1CType asn1_type_dot3EdcaParameterSet[];

typedef int dot3ChannelAccess80211;

enum {
  dot3ChannelAccess80211_continuous = 0,
  dot3ChannelAccess80211_alternatingSCH = 1,
  dot3ChannelAccess80211_alternatingCCH = 2,
};

extern const ASN1CType asn1_type_dot3ChannelAccess80211[];

typedef dot3IPv6Address dot3SecondaryDns;

#define asn1_type_dot3SecondaryDns asn1_type_dot3IPv6Address

typedef dot3MACaddress dot3GatewayMacAddress;

#define asn1_type_dot3GatewayMacAddress asn1_type_dot3MACaddress

typedef int dot3SrvAdvMessageType;

enum {
  dot3SrvAdvMessageType_saMessage = 0,
  dot3SrvAdvMessageType_sarMessage = 1,
};

extern const ASN1CType asn1_type_dot3SrvAdvMessageType[];

typedef int dot3RsvAdvPrtVersion;

extern const ASN1CType asn1_type_dot3RsvAdvPrtVersion[];

typedef struct dot3SrvAdvPrtVersion {
  dot3SrvAdvMessageType messageID;
  dot3RsvAdvPrtVersion rsvAdvPrtVersion;
} dot3SrvAdvPrtVersion;


extern const ASN1CType asn1_type_dot3SrvAdvPrtVersion[];

typedef int dot3SrvAdvID;

extern const ASN1CType asn1_type_dot3SrvAdvID[];

typedef int dot3SrvAdvContentCount;

extern const ASN1CType asn1_type_dot3SrvAdvContentCount[];

typedef struct dot3SrvAdvChangeCount {
  dot3SrvAdvID saID;
  dot3SrvAdvContentCount contentCount;
} dot3SrvAdvChangeCount;


extern const ASN1CType asn1_type_dot3SrvAdvChangeCount[];

typedef struct dot3SrvAdvMsgHeaderExt {
  dot3RefExt extensionId;
  ASN1OpenType value;
} dot3SrvAdvMsgHeaderExt;


extern const ASN1CType asn1_type_dot3SrvAdvMsgHeaderExt[];

typedef struct dot3SrvAdvMsgHeaderExts {
  dot3SrvAdvMsgHeaderExt *tab;
  size_t count;
} dot3SrvAdvMsgHeaderExts;

extern const ASN1CType asn1_type_dot3SrvAdvMsgHeaderExts[];

typedef int dot3ChannelIndex;

enum {
  dot3ChannelIndex_notUsed = 0,
  dot3ChannelIndex_firstEntry = 1,
};

extern const ASN1CType asn1_type_dot3ChannelIndex[];

typedef ASN1Null dot3MandApp;

extern const ASN1CType asn1_type_dot3MandApp[];

typedef ASN1Null dot3ReplyAddress;

extern const ASN1CType asn1_type_dot3ReplyAddress[];

typedef struct dot3ServiceInfoExt {
  dot3RefExt extensionId;
  ASN1OpenType value;
} dot3ServiceInfoExt;


extern const ASN1CType asn1_type_dot3ServiceInfoExt[];

typedef struct dot3ServiceInfoExts {
  dot3ServiceInfoExt *tab;
  size_t count;
} dot3ServiceInfoExts;

extern const ASN1CType asn1_type_dot3ServiceInfoExts[];

typedef struct dot3ChannelOptions {
  BOOL mandApp_option;
  BOOL serviceProviderPort_option;
  BOOL extensions_option;
  dot3ServiceInfoExts extensions;
} dot3ChannelOptions;


extern const ASN1CType asn1_type_dot3ChannelOptions[];

typedef struct dot3ServiceInfo {
  dot3VarLengthNumber serviceID;
  dot3ChannelIndex channelIndex;
  dot3ChannelOptions chOptions;
} dot3ServiceInfo;


extern const ASN1CType asn1_type_dot3ServiceInfo[];

typedef struct dot3ServiceInfos {
  dot3ServiceInfo *tab;
  size_t count;
} dot3ServiceInfos;

extern const ASN1CType asn1_type_dot3ServiceInfos[];

typedef int dot3OperatingClass80211;

extern const ASN1CType asn1_type_dot3OperatingClass80211[];

typedef struct dot3WsaChInfoDataRate {
  ASN1BitString adaptable;
  int dataRate;
} dot3WsaChInfoDataRate;


extern const ASN1CType asn1_type_dot3WsaChInfoDataRate[];

typedef struct dot3ChannelInfoExt {
  dot3RefExt extensionId;
  ASN1OpenType value;
} dot3ChannelInfoExt;


extern const ASN1CType asn1_type_dot3ChannelInfoExt[];

typedef struct dot3ChannelInfoExts {
  dot3ChannelInfoExt *tab;
  size_t count;
} dot3ChannelInfoExts;

extern const ASN1CType asn1_type_dot3ChannelInfoExts[];

typedef struct dot3ChInfoOptions {
  BOOL option1_option;
  BOOL option2_option;
  BOOL option3_option;
  BOOL option4_option;
  BOOL option5_option;
  BOOL option6_option;
  BOOL option7_option;
  BOOL extensions_option;
  dot3ChannelInfoExts extensions;
} dot3ChInfoOptions;


extern const ASN1CType asn1_type_dot3ChInfoOptions[];

typedef struct dot3ChannelInfo {
  dot3OperatingClass80211 operatingClass;
  dot3ChannelNumber80211 channelNumber;
  dot3TXpower80211 powerLevel;
  dot3WsaChInfoDataRate dataRate;
  dot3ChInfoOptions extensions;
} dot3ChannelInfo;


extern const ASN1CType asn1_type_dot3ChannelInfo[];

typedef struct dot3ChannelInfos {
  dot3ChannelInfo *tab;
  size_t count;
} dot3ChannelInfos;

extern const ASN1CType asn1_type_dot3ChannelInfos[];

typedef int dot3RouterLifetime;

extern const ASN1CType asn1_type_dot3RouterLifetime[];

typedef ASN1String dot3IpV6Prefix;

extern const ASN1CType asn1_type_dot3IpV6Prefix[];

typedef int dot3IpV6PrefixLength;

extern const ASN1CType asn1_type_dot3IpV6PrefixLength[];

typedef struct dot3RoutAdvertExt {
  dot3RefExt extensionId;
  ASN1OpenType value;
} dot3RoutAdvertExt;


extern const ASN1CType asn1_type_dot3RoutAdvertExt[];

typedef struct dot3RoutAdvertExts {
  dot3RoutAdvertExt *tab;
  size_t count;
} dot3RoutAdvertExts;

extern const ASN1CType asn1_type_dot3RoutAdvertExts[];

typedef struct dot3RoutingAdvertisement {
  dot3RouterLifetime lifetime;
  dot3IpV6Prefix ipPrefix;
  dot3IpV6PrefixLength ipPrefixLength;
  dot3IPv6Address defaultGateway;
  dot3IPv6Address primaryDns;
  dot3RoutAdvertExts extensions;
} dot3RoutingAdvertisement;


extern const ASN1CType asn1_type_dot3RoutingAdvertisement[];

typedef struct dot3SrvAdvBody {
  dot3SrvAdvChangeCount changeCount;
  BOOL extensions_option;
  dot3SrvAdvMsgHeaderExts extensions;
  BOOL serviceInfos_option;
  dot3ServiceInfos serviceInfos;
  BOOL channelInfos_option;
  dot3ChannelInfos channelInfos;
  BOOL routingAdvertisement_option;
  dot3RoutingAdvertisement routingAdvertisement;
} dot3SrvAdvBody;


extern const ASN1CType asn1_type_dot3SrvAdvBody[];

typedef struct dot3SrvAdvMsg {
  dot3SrvAdvPrtVersion version;
  dot3SrvAdvBody body;
} dot3SrvAdvMsg;


extern const ASN1CType asn1_type_dot3SrvAdvMsg[];

typedef int dot3ShortMsgVersion;

extern const ASN1CType asn1_type_dot3ShortMsgVersion[];

typedef struct dot3ShortMsgNextension {
  dot3RefExt extensionId;
  ASN1OpenType value;
} dot3ShortMsgNextension;


extern const ASN1CType asn1_type_dot3ShortMsgNextension[];

typedef struct dot3ShortMsgNextensions {
  dot3ShortMsgNextension *tab;
  size_t count;
} dot3ShortMsgNextensions;

extern const ASN1CType asn1_type_dot3ShortMsgNextensions[];

typedef struct dot3NullNetworking {
  dot3ShortMsgVersion version;
  BOOL nExtensions_option;
  dot3ShortMsgNextensions nExtensions;
} dot3NullNetworking;


extern const ASN1CType asn1_type_dot3NullNetworking[];

typedef struct dot3NoSubtypeProcessing {
  ASN1BitString optBit;
  dot3ShortMsgVersion version;
} dot3NoSubtypeProcessing;


extern const ASN1CType asn1_type_dot3NoSubtypeProcessing[];

typedef enum {
  dot3ShortMsgSubtype_nullNetworking,
  dot3ShortMsgSubtype_subTypeReserved1,
  dot3ShortMsgSubtype_subTypeReserved2,
  dot3ShortMsgSubtype_subTypeReserved3,
  dot3ShortMsgSubtype_subTypeReserved4,
  dot3ShortMsgSubtype_subTypeReserved5,
  dot3ShortMsgSubtype_subTypeReserved6,
  dot3ShortMsgSubtype_subTypeReserved7,
  dot3ShortMsgSubtype_subTypeReserved8,
  dot3ShortMsgSubtype_subTypeReserved9,
  dot3ShortMsgSubtype_subTypeReserved19,
  dot3ShortMsgSubtype_subTypeReserved11,
  dot3ShortMsgSubtype_subTypeReserved12,
  dot3ShortMsgSubtype_subTypeReserved13,
  dot3ShortMsgSubtype_subTypeReserved14,
  dot3ShortMsgSubtype_subTypeReserved15,
} dot3ShortMsgSubtype_choice;

typedef struct dot3ShortMsgSubtype {
  dot3ShortMsgSubtype_choice choice;
  union {
    dot3NullNetworking nullNetworking;
    dot3NoSubtypeProcessing subTypeReserved1;
    dot3NoSubtypeProcessing subTypeReserved2;
    dot3NoSubtypeProcessing subTypeReserved3;
    dot3NoSubtypeProcessing subTypeReserved4;
    dot3NoSubtypeProcessing subTypeReserved5;
    dot3NoSubtypeProcessing subTypeReserved6;
    dot3NoSubtypeProcessing subTypeReserved7;
    dot3NoSubtypeProcessing subTypeReserved8;
    dot3NoSubtypeProcessing subTypeReserved9;
    dot3NoSubtypeProcessing subTypeReserved19;
    dot3NoSubtypeProcessing subTypeReserved11;
    dot3NoSubtypeProcessing subTypeReserved12;
    dot3NoSubtypeProcessing subTypeReserved13;
    dot3NoSubtypeProcessing subTypeReserved14;
    dot3NoSubtypeProcessing subTypeReserved15;
  } u;
} dot3ShortMsgSubtype;

extern const ASN1CType asn1_type_dot3ShortMsgSubtype[];

typedef struct dot3ShortMsgTextension {
  dot3RefExt extensionId;
  ASN1OpenType value;
} dot3ShortMsgTextension;


extern const ASN1CType asn1_type_dot3ShortMsgTextension[];

typedef struct dot3ShortMsgTextensions {
  dot3ShortMsgTextension *tab;
  size_t count;
} dot3ShortMsgTextensions;

extern const ASN1CType asn1_type_dot3ShortMsgTextensions[];

typedef struct dot3ShortMsgBcPDU {
  dot3VarLengthNumber destAddress;
  BOOL tExtensions_option;
  dot3ShortMsgTextensions tExtensions;
} dot3ShortMsgBcPDU;


extern const ASN1CType asn1_type_dot3ShortMsgBcPDU[];

typedef ASN1BitString dot3NoTpidProcessing;

extern const ASN1CType asn1_type_dot3NoTpidProcessing[];

typedef enum {
  dot3ShortMsgTpdus_bcMode,
  dot3ShortMsgTpdus_tpidReserved1,
  dot3ShortMsgTpdus_tpidReserved2,
  dot3ShortMsgTpdus_tpidReserved3,
  dot3ShortMsgTpdus_tpidReserved4,
  dot3ShortMsgTpdus_tpidReserved5,
  dot3ShortMsgTpdus_tpidReserved6,
  dot3ShortMsgTpdus_tpidReserved7,
  dot3ShortMsgTpdus_tpidReserved8,
  dot3ShortMsgTpdus_tpidReserved9,
  dot3ShortMsgTpdus_tpidReserved10,
  dot3ShortMsgTpdus_tpidReserved11,
  dot3ShortMsgTpdus_tpidReserved12,
  dot3ShortMsgTpdus_tpidReserved13,
  dot3ShortMsgTpdus_tpidReserved14,
  dot3ShortMsgTpdus_tpidReserved15,
  dot3ShortMsgTpdus_tpidReserved16,
  dot3ShortMsgTpdus_tpidReserved17,
  dot3ShortMsgTpdus_tpidReserved18,
  dot3ShortMsgTpdus_tpidReserved19,
  dot3ShortMsgTpdus_tpidReserved20,
  dot3ShortMsgTpdus_tpidReserved21,
  dot3ShortMsgTpdus_tpidReserved22,
  dot3ShortMsgTpdus_tpidReserved23,
  dot3ShortMsgTpdus_tpidReserved24,
  dot3ShortMsgTpdus_tpidReserved25,
  dot3ShortMsgTpdus_tpidReserved26,
  dot3ShortMsgTpdus_tpidReserved27,
  dot3ShortMsgTpdus_tpidReserved28,
  dot3ShortMsgTpdus_tpidReserved29,
  dot3ShortMsgTpdus_tpidReserved30,
  dot3ShortMsgTpdus_tpidReserved31,
  dot3ShortMsgTpdus_tpidReserved32,
  dot3ShortMsgTpdus_tpidReserved33,
  dot3ShortMsgTpdus_tpidReserved34,
  dot3ShortMsgTpdus_tpidReserved35,
  dot3ShortMsgTpdus_tpidReserved36,
  dot3ShortMsgTpdus_tpidReserved37,
  dot3ShortMsgTpdus_tpidReserved38,
  dot3ShortMsgTpdus_tpidReserved39,
  dot3ShortMsgTpdus_tpidReserved40,
  dot3ShortMsgTpdus_tpidReserved41,
  dot3ShortMsgTpdus_tpidReserved42,
  dot3ShortMsgTpdus_tpidReserved43,
  dot3ShortMsgTpdus_tpidReserved44,
  dot3ShortMsgTpdus_tpidReserved45,
  dot3ShortMsgTpdus_tpidReserved46,
  dot3ShortMsgTpdus_tpidReserved47,
  dot3ShortMsgTpdus_tpidReserved48,
  dot3ShortMsgTpdus_tpidReserved49,
  dot3ShortMsgTpdus_tpidReserved50,
  dot3ShortMsgTpdus_tpidReserved51,
  dot3ShortMsgTpdus_tpidReserved52,
  dot3ShortMsgTpdus_tpidReserved53,
  dot3ShortMsgTpdus_tpidReserved54,
  dot3ShortMsgTpdus_tpidReserved55,
  dot3ShortMsgTpdus_tpidReserved56,
  dot3ShortMsgTpdus_tpidReserved57,
  dot3ShortMsgTpdus_tpidReserved58,
  dot3ShortMsgTpdus_tpidReserved59,
  dot3ShortMsgTpdus_tpidReserved60,
  dot3ShortMsgTpdus_tpidReserved61,
  dot3ShortMsgTpdus_tpidReserved62,
  dot3ShortMsgTpdus_tpidReserved63,
  dot3ShortMsgTpdus_tpidReserved64,
  dot3ShortMsgTpdus_tpidReserved65,
  dot3ShortMsgTpdus_tpidReserved66,
  dot3ShortMsgTpdus_tpidReserved67,
  dot3ShortMsgTpdus_tpidReserved68,
  dot3ShortMsgTpdus_tpidReserved69,
  dot3ShortMsgTpdus_tpidReserved70,
  dot3ShortMsgTpdus_tpidReserved71,
  dot3ShortMsgTpdus_tpidReserved72,
  dot3ShortMsgTpdus_tpidReserved73,
  dot3ShortMsgTpdus_tpidReserved74,
  dot3ShortMsgTpdus_tpidReserved75,
  dot3ShortMsgTpdus_tpidReserved76,
  dot3ShortMsgTpdus_tpidReserved77,
  dot3ShortMsgTpdus_tpidReserved78,
  dot3ShortMsgTpdus_tpidReserved79,
  dot3ShortMsgTpdus_tpidReserved80,
  dot3ShortMsgTpdus_tpidReserved81,
  dot3ShortMsgTpdus_tpidReserved82,
  dot3ShortMsgTpdus_tpidReserved83,
  dot3ShortMsgTpdus_tpidReserved84,
  dot3ShortMsgTpdus_tpidReserved85,
  dot3ShortMsgTpdus_tpidReserved86,
  dot3ShortMsgTpdus_tpidReserved87,
  dot3ShortMsgTpdus_tpidReserved88,
  dot3ShortMsgTpdus_tpidReserved89,
  dot3ShortMsgTpdus_tpidReserved90,
  dot3ShortMsgTpdus_tpidReserved91,
  dot3ShortMsgTpdus_tpidReserved92,
  dot3ShortMsgTpdus_tpidReserved93,
  dot3ShortMsgTpdus_tpidReserved94,
  dot3ShortMsgTpdus_tpidReserved95,
  dot3ShortMsgTpdus_tpidReserved96,
  dot3ShortMsgTpdus_tpidReserved97,
  dot3ShortMsgTpdus_tpidReserved98,
  dot3ShortMsgTpdus_tpidReserved99,
  dot3ShortMsgTpdus_tpidReserved100,
  dot3ShortMsgTpdus_tpidReserved101,
  dot3ShortMsgTpdus_tpidReserved102,
  dot3ShortMsgTpdus_tpidReserved103,
  dot3ShortMsgTpdus_tpidReserved104,
  dot3ShortMsgTpdus_tpidReserved105,
  dot3ShortMsgTpdus_tpidReserved106,
  dot3ShortMsgTpdus_tpidReserved107,
  dot3ShortMsgTpdus_tpidReserved108,
  dot3ShortMsgTpdus_tpidReserved109,
  dot3ShortMsgTpdus_tpidReserved110,
  dot3ShortMsgTpdus_tpidReserved111,
  dot3ShortMsgTpdus_tpidReserved112,
  dot3ShortMsgTpdus_tpidReserved113,
  dot3ShortMsgTpdus_tpidReserved114,
  dot3ShortMsgTpdus_tpidReserved115,
  dot3ShortMsgTpdus_tpidReserved116,
  dot3ShortMsgTpdus_tpidReserved117,
  dot3ShortMsgTpdus_tpidReserved118,
  dot3ShortMsgTpdus_tpidReserved119,
  dot3ShortMsgTpdus_tpidReserved120,
  dot3ShortMsgTpdus_tpidReserved121,
  dot3ShortMsgTpdus_tpidReserved122,
  dot3ShortMsgTpdus_tpidReserved123,
  dot3ShortMsgTpdus_tpidReserved124,
  dot3ShortMsgTpdus_tpidReserved125,
  dot3ShortMsgTpdus_tpidReserved126,
  dot3ShortMsgTpdus_tpidReserved127,
} dot3ShortMsgTpdus_choice;

typedef struct dot3ShortMsgTpdus {
  dot3ShortMsgTpdus_choice choice;
  union {
    dot3ShortMsgBcPDU bcMode;
    dot3NoTpidProcessing tpidReserved1;
    dot3NoTpidProcessing tpidReserved2;
    dot3NoTpidProcessing tpidReserved3;
    dot3NoTpidProcessing tpidReserved4;
    dot3NoTpidProcessing tpidReserved5;
    dot3NoTpidProcessing tpidReserved6;
    dot3NoTpidProcessing tpidReserved7;
    dot3NoTpidProcessing tpidReserved8;
    dot3NoTpidProcessing tpidReserved9;
    dot3NoTpidProcessing tpidReserved10;
    dot3NoTpidProcessing tpidReserved11;
    dot3NoTpidProcessing tpidReserved12;
    dot3NoTpidProcessing tpidReserved13;
    dot3NoTpidProcessing tpidReserved14;
    dot3NoTpidProcessing tpidReserved15;
    dot3NoTpidProcessing tpidReserved16;
    dot3NoTpidProcessing tpidReserved17;
    dot3NoTpidProcessing tpidReserved18;
    dot3NoTpidProcessing tpidReserved19;
    dot3NoTpidProcessing tpidReserved20;
    dot3NoTpidProcessing tpidReserved21;
    dot3NoTpidProcessing tpidReserved22;
    dot3NoTpidProcessing tpidReserved23;
    dot3NoTpidProcessing tpidReserved24;
    dot3NoTpidProcessing tpidReserved25;
    dot3NoTpidProcessing tpidReserved26;
    dot3NoTpidProcessing tpidReserved27;
    dot3NoTpidProcessing tpidReserved28;
    dot3NoTpidProcessing tpidReserved29;
    dot3NoTpidProcessing tpidReserved30;
    dot3NoTpidProcessing tpidReserved31;
    dot3NoTpidProcessing tpidReserved32;
    dot3NoTpidProcessing tpidReserved33;
    dot3NoTpidProcessing tpidReserved34;
    dot3NoTpidProcessing tpidReserved35;
    dot3NoTpidProcessing tpidReserved36;
    dot3NoTpidProcessing tpidReserved37;
    dot3NoTpidProcessing tpidReserved38;
    dot3NoTpidProcessing tpidReserved39;
    dot3NoTpidProcessing tpidReserved40;
    dot3NoTpidProcessing tpidReserved41;
    dot3NoTpidProcessing tpidReserved42;
    dot3NoTpidProcessing tpidReserved43;
    dot3NoTpidProcessing tpidReserved44;
    dot3NoTpidProcessing tpidReserved45;
    dot3NoTpidProcessing tpidReserved46;
    dot3NoTpidProcessing tpidReserved47;
    dot3NoTpidProcessing tpidReserved48;
    dot3NoTpidProcessing tpidReserved49;
    dot3NoTpidProcessing tpidReserved50;
    dot3NoTpidProcessing tpidReserved51;
    dot3NoTpidProcessing tpidReserved52;
    dot3NoTpidProcessing tpidReserved53;
    dot3NoTpidProcessing tpidReserved54;
    dot3NoTpidProcessing tpidReserved55;
    dot3NoTpidProcessing tpidReserved56;
    dot3NoTpidProcessing tpidReserved57;
    dot3NoTpidProcessing tpidReserved58;
    dot3NoTpidProcessing tpidReserved59;
    dot3NoTpidProcessing tpidReserved60;
    dot3NoTpidProcessing tpidReserved61;
    dot3NoTpidProcessing tpidReserved62;
    dot3NoTpidProcessing tpidReserved63;
    dot3NoTpidProcessing tpidReserved64;
    dot3NoTpidProcessing tpidReserved65;
    dot3NoTpidProcessing tpidReserved66;
    dot3NoTpidProcessing tpidReserved67;
    dot3NoTpidProcessing tpidReserved68;
    dot3NoTpidProcessing tpidReserved69;
    dot3NoTpidProcessing tpidReserved70;
    dot3NoTpidProcessing tpidReserved71;
    dot3NoTpidProcessing tpidReserved72;
    dot3NoTpidProcessing tpidReserved73;
    dot3NoTpidProcessing tpidReserved74;
    dot3NoTpidProcessing tpidReserved75;
    dot3NoTpidProcessing tpidReserved76;
    dot3NoTpidProcessing tpidReserved77;
    dot3NoTpidProcessing tpidReserved78;
    dot3NoTpidProcessing tpidReserved79;
    dot3NoTpidProcessing tpidReserved80;
    dot3NoTpidProcessing tpidReserved81;
    dot3NoTpidProcessing tpidReserved82;
    dot3NoTpidProcessing tpidReserved83;
    dot3NoTpidProcessing tpidReserved84;
    dot3NoTpidProcessing tpidReserved85;
    dot3NoTpidProcessing tpidReserved86;
    dot3NoTpidProcessing tpidReserved87;
    dot3NoTpidProcessing tpidReserved88;
    dot3NoTpidProcessing tpidReserved89;
    dot3NoTpidProcessing tpidReserved90;
    dot3NoTpidProcessing tpidReserved91;
    dot3NoTpidProcessing tpidReserved92;
    dot3NoTpidProcessing tpidReserved93;
    dot3NoTpidProcessing tpidReserved94;
    dot3NoTpidProcessing tpidReserved95;
    dot3NoTpidProcessing tpidReserved96;
    dot3NoTpidProcessing tpidReserved97;
    dot3NoTpidProcessing tpidReserved98;
    dot3NoTpidProcessing tpidReserved99;
    dot3NoTpidProcessing tpidReserved100;
    dot3NoTpidProcessing tpidReserved101;
    dot3NoTpidProcessing tpidReserved102;
    dot3NoTpidProcessing tpidReserved103;
    dot3NoTpidProcessing tpidReserved104;
    dot3NoTpidProcessing tpidReserved105;
    dot3NoTpidProcessing tpidReserved106;
    dot3NoTpidProcessing tpidReserved107;
    dot3NoTpidProcessing tpidReserved108;
    dot3NoTpidProcessing tpidReserved109;
    dot3NoTpidProcessing tpidReserved110;
    dot3NoTpidProcessing tpidReserved111;
    dot3NoTpidProcessing tpidReserved112;
    dot3NoTpidProcessing tpidReserved113;
    dot3NoTpidProcessing tpidReserved114;
    dot3NoTpidProcessing tpidReserved115;
    dot3NoTpidProcessing tpidReserved116;
    dot3NoTpidProcessing tpidReserved117;
    dot3NoTpidProcessing tpidReserved118;
    dot3NoTpidProcessing tpidReserved119;
    dot3NoTpidProcessing tpidReserved120;
    dot3NoTpidProcessing tpidReserved121;
    dot3NoTpidProcessing tpidReserved122;
    dot3NoTpidProcessing tpidReserved123;
    dot3NoTpidProcessing tpidReserved124;
    dot3NoTpidProcessing tpidReserved125;
    dot3NoTpidProcessing tpidReserved126;
    dot3NoTpidProcessing tpidReserved127;
  } u;
} dot3ShortMsgTpdus;

extern const ASN1CType asn1_type_dot3ShortMsgTpdus[];

typedef ASN1String dot3ShortMsgData;

extern const ASN1CType asn1_type_dot3ShortMsgData[];

typedef struct dot3ShortMsgNpdu {
  dot3ShortMsgSubtype subtype;
  dot3ShortMsgTpdus transport;
  dot3ShortMsgData body;
} dot3ShortMsgNpdu;


extern const ASN1CType asn1_type_dot3ShortMsgNpdu[];

typedef ASN1Null dot3DummyExtension;

extern const ASN1CType asn1_type_dot3DummyExtension[];

#ifdef  __cplusplus
}
#endif

#endif /* _FFASN1_FFASN1_DOT3_2016_H */
