/* Automatically generated file - do not edit */
#ifndef _FFASN1_FFASN1_DOT2_2016_H
#define _FFASN1_FFASN1_DOT2_2016_H

#include "asn1defs.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef int dot2Uint3;

extern const ASN1CType asn1_type_dot2Uint3[];

typedef int dot2Uint8;

extern const ASN1CType asn1_type_dot2Uint8[];

typedef int dot2Uint16;

extern const ASN1CType asn1_type_dot2Uint16[];

typedef unsigned int dot2Uint32;

extern const ASN1CType asn1_type_dot2Uint32[];

typedef ASN1Integer dot2Uint64;

extern const ASN1CType asn1_type_dot2Uint64[];

typedef struct dot2SequenceOfUint8 {
  dot2Uint8 *tab;
  size_t count;
} dot2SequenceOfUint8;

extern const ASN1CType asn1_type_dot2SequenceOfUint8[];

typedef struct dot2SequenceOfUint16 {
  dot2Uint16 *tab;
  size_t count;
} dot2SequenceOfUint16;

extern const ASN1CType asn1_type_dot2SequenceOfUint16[];

typedef ASN1String dot2Opaque;

extern const ASN1CType asn1_type_dot2Opaque[];

typedef ASN1String dot2HashedId10;

extern const ASN1CType asn1_type_dot2HashedId10[];

typedef ASN1String dot2HashedId8;

extern const ASN1CType asn1_type_dot2HashedId8[];

typedef ASN1String dot2HashedId3;

extern const ASN1CType asn1_type_dot2HashedId3[];

typedef struct dot2SequenceOfHashedId3 {
  dot2HashedId3 *tab;
  size_t count;
} dot2SequenceOfHashedId3;

extern const ASN1CType asn1_type_dot2SequenceOfHashedId3[];

typedef dot2Uint32 dot2Time32;

#define asn1_type_dot2Time32 asn1_type_dot2Uint32

typedef dot2Uint64 dot2Time64;

#define asn1_type_dot2Time64 asn1_type_dot2Uint64

typedef enum {
  dot2Duration_microseconds,
  dot2Duration_milliseconds,
  dot2Duration_seconds,
  dot2Duration_minutes,
  dot2Duration_hours,
  dot2Duration_sixtyHours,
  dot2Duration_years,
} dot2Duration_choice;

typedef struct dot2Duration {
  dot2Duration_choice choice;
  union {
    dot2Uint16 microseconds;
    dot2Uint16 milliseconds;
    dot2Uint16 seconds;
    dot2Uint16 minutes;
    dot2Uint16 hours;
    dot2Uint16 sixtyHours;
    dot2Uint16 years;
  } u;
} dot2Duration;

extern const ASN1CType asn1_type_dot2Duration[];

typedef struct dot2ValidityPeriod {
  dot2Time32 start;
  dot2Duration duration;
} dot2ValidityPeriod;


extern const ASN1CType asn1_type_dot2ValidityPeriod[];

typedef int dot2NinetyDegreeInt;

enum {
  dot2NinetyDegreeInt_min = -900000000,
  dot2NinetyDegreeInt_max = 900000000,
  dot2NinetyDegreeInt_unknown = 900000001,
};

extern const ASN1CType asn1_type_dot2NinetyDegreeInt[];

typedef dot2NinetyDegreeInt dot2Latitude;

extern const ASN1CType asn1_type_dot2Latitude[];

typedef int dot2OneEightyDegreeInt;

enum {
  dot2OneEightyDegreeInt_min = -1799999999,
  dot2OneEightyDegreeInt_max = 1800000000,
  dot2OneEightyDegreeInt_unknown = 1800000001,
};

extern const ASN1CType asn1_type_dot2OneEightyDegreeInt[];

typedef dot2OneEightyDegreeInt dot2Longitude;

extern const ASN1CType asn1_type_dot2Longitude[];

typedef struct dot2TwoDLocation {
  dot2Latitude latitude;
  dot2Longitude longitude;
} dot2TwoDLocation;


extern const ASN1CType asn1_type_dot2TwoDLocation[];

typedef struct dot2CircularRegion {
  dot2TwoDLocation center;
  dot2Uint16 radius;
} dot2CircularRegion;


extern const ASN1CType asn1_type_dot2CircularRegion[];

typedef struct dot2RectangularRegion {
  dot2TwoDLocation northWest;
  dot2TwoDLocation southEast;
} dot2RectangularRegion;


extern const ASN1CType asn1_type_dot2RectangularRegion[];

typedef struct dot2SequenceOfRectangularRegion {
  dot2RectangularRegion *tab;
  size_t count;
} dot2SequenceOfRectangularRegion;

extern const ASN1CType asn1_type_dot2SequenceOfRectangularRegion[];

typedef struct dot2PolygonalRegion {
  dot2TwoDLocation *tab;
  size_t count;
} dot2PolygonalRegion;

extern const ASN1CType asn1_type_dot2PolygonalRegion[];

typedef dot2Uint16 dot2CountryOnly;

extern const ASN1CType asn1_type_dot2CountryOnly[];

typedef struct dot2CountryAndRegions {
  dot2CountryOnly countryOnly;
  dot2SequenceOfUint8 regions;
} dot2CountryAndRegions;


extern const ASN1CType asn1_type_dot2CountryAndRegions[];

typedef struct dot2RegionAndSubregions {
  dot2Uint8 region;
  dot2SequenceOfUint16 subregions;
} dot2RegionAndSubregions;


extern const ASN1CType asn1_type_dot2RegionAndSubregions[];

typedef struct dot2SequenceOfRegionAndSubregions {
  dot2RegionAndSubregions *tab;
  size_t count;
} dot2SequenceOfRegionAndSubregions;

extern const ASN1CType asn1_type_dot2SequenceOfRegionAndSubregions[];

typedef struct dot2CountryAndSubregions {
  dot2CountryOnly country;
  dot2SequenceOfRegionAndSubregions regionAndSubregions;
} dot2CountryAndSubregions;


extern const ASN1CType asn1_type_dot2CountryAndSubregions[];

typedef enum {
  dot2IdentifiedRegion_countryOnly,
  dot2IdentifiedRegion_countryAndRegions,
  dot2IdentifiedRegion_countryAndSubregions,
} dot2IdentifiedRegion_choice;

typedef struct dot2IdentifiedRegion {
  dot2IdentifiedRegion_choice choice;
  union {
    dot2CountryOnly countryOnly;
    dot2CountryAndRegions countryAndRegions;
    dot2CountryAndSubregions countryAndSubregions;
  } u;
} dot2IdentifiedRegion;

extern const ASN1CType asn1_type_dot2IdentifiedRegion[];

typedef struct dot2SequenceOfIdentifiedRegion {
  dot2IdentifiedRegion *tab;
  size_t count;
} dot2SequenceOfIdentifiedRegion;

extern const ASN1CType asn1_type_dot2SequenceOfIdentifiedRegion[];

typedef enum {
  dot2GeographicRegion_circularRegion,
  dot2GeographicRegion_rectangularRegion,
  dot2GeographicRegion_polygonalRegion,
  dot2GeographicRegion_identifiedRegion,
} dot2GeographicRegion_choice;

typedef struct dot2GeographicRegion {
  dot2GeographicRegion_choice choice;
  union {
    dot2CircularRegion circularRegion;
    dot2SequenceOfRectangularRegion rectangularRegion;
    dot2PolygonalRegion polygonalRegion;
    dot2SequenceOfIdentifiedRegion identifiedRegion;
  } u;
} dot2GeographicRegion;

extern const ASN1CType asn1_type_dot2GeographicRegion[];

typedef dot2Uint16 dot2ElevInt;

#define asn1_type_dot2ElevInt asn1_type_dot2Uint16

typedef dot2ElevInt dot2Elevation;

extern const ASN1CType asn1_type_dot2Elevation[];

typedef struct dot2ThreeDLocation {
  dot2Latitude latitude;
  dot2Longitude longitude;
  dot2Elevation elevation;
} dot2ThreeDLocation;


extern const ASN1CType asn1_type_dot2ThreeDLocation[];

typedef int dot2NinetyDegreeInt_1;

enum {
  dot2NinetyDegreeInt_1_min = -900000000,
  dot2NinetyDegreeInt_1_max = 900000000,
  dot2NinetyDegreeInt_1_unknown = 900000001,
};

extern const ASN1CType asn1_type_dot2NinetyDegreeInt_1[];

typedef dot2NinetyDegreeInt_1 dot2KnownLatitude;

#define asn1_type_dot2KnownLatitude asn1_type_dot2NinetyDegreeInt_1

typedef int dot2NinetyDegreeInt_2;

enum {
  dot2NinetyDegreeInt_2_min = -900000000,
  dot2NinetyDegreeInt_2_max = 900000000,
  dot2NinetyDegreeInt_2_unknown = 900000001,
};

extern const ASN1CType asn1_type_dot2NinetyDegreeInt_2[];

typedef dot2NinetyDegreeInt_2 dot2UnknownLatitude;

#define asn1_type_dot2UnknownLatitude asn1_type_dot2NinetyDegreeInt_2

typedef int dot2OneEightyDegreeInt_1;

enum {
  dot2OneEightyDegreeInt_1_min = -1799999999,
  dot2OneEightyDegreeInt_1_max = 1800000000,
  dot2OneEightyDegreeInt_1_unknown = 1800000001,
};

extern const ASN1CType asn1_type_dot2OneEightyDegreeInt_1[];

typedef dot2OneEightyDegreeInt_1 dot2KnownLongitude;

#define asn1_type_dot2KnownLongitude asn1_type_dot2OneEightyDegreeInt_1

typedef int dot2OneEightyDegreeInt_2;

enum {
  dot2OneEightyDegreeInt_2_min = -1799999999,
  dot2OneEightyDegreeInt_2_max = 1800000000,
  dot2OneEightyDegreeInt_2_unknown = 1800000001,
};

extern const ASN1CType asn1_type_dot2OneEightyDegreeInt_2[];

typedef dot2OneEightyDegreeInt_2 dot2UnknownLongitude;

#define asn1_type_dot2UnknownLongitude asn1_type_dot2OneEightyDegreeInt_2

typedef struct dot2EccP256CurvePoint_1 {
  ASN1String x;
  ASN1String y;
} dot2EccP256CurvePoint_1;


extern const ASN1CType asn1_type_dot2EccP256CurvePoint_1[];

typedef enum {
  dot2EccP256CurvePoint_x_only,
  dot2EccP256CurvePoint_fill,
  dot2EccP256CurvePoint_compressed_y_0,
  dot2EccP256CurvePoint_compressed_y_1,
  dot2EccP256CurvePoint_uncompressedP256,
} dot2EccP256CurvePoint_choice;

typedef struct dot2EccP256CurvePoint {
  dot2EccP256CurvePoint_choice choice;
  union {
    ASN1String x_only;
    ASN1String compressed_y_0;
    ASN1String compressed_y_1;
    dot2EccP256CurvePoint_1 uncompressedP256;
  } u;
} dot2EccP256CurvePoint;

extern const ASN1CType asn1_type_dot2EccP256CurvePoint[];

typedef struct dot2EcdsaP256Signature {
  dot2EccP256CurvePoint rSig;
  ASN1String sSig;
} dot2EcdsaP256Signature;


extern const ASN1CType asn1_type_dot2EcdsaP256Signature[];

typedef struct dot2EccP384CurvePoint_1 {
  ASN1String x;
  ASN1String y;
} dot2EccP384CurvePoint_1;


extern const ASN1CType asn1_type_dot2EccP384CurvePoint_1[];

typedef enum {
  dot2EccP384CurvePoint_x_only,
  dot2EccP384CurvePoint_fill,
  dot2EccP384CurvePoint_compressed_y_0,
  dot2EccP384CurvePoint_compressed_y_1,
  dot2EccP384CurvePoint_uncompressedP384,
} dot2EccP384CurvePoint_choice;

typedef struct dot2EccP384CurvePoint {
  dot2EccP384CurvePoint_choice choice;
  union {
    ASN1String x_only;
    ASN1String compressed_y_0;
    ASN1String compressed_y_1;
    dot2EccP384CurvePoint_1 uncompressedP384;
  } u;
} dot2EccP384CurvePoint;

extern const ASN1CType asn1_type_dot2EccP384CurvePoint[];

typedef struct dot2EcdsaP384Signature {
  dot2EccP384CurvePoint rSig;
  ASN1String sSig;
} dot2EcdsaP384Signature;


extern const ASN1CType asn1_type_dot2EcdsaP384Signature[];

typedef enum {
  dot2Signature_ecdsaNistP256Signature,
  dot2Signature_ecdsaBrainpoolP256r1Signature,
  dot2Signature_ecdsaBrainpoolP384r1Signature,
} dot2Signature_choice;

typedef struct dot2Signature {
  dot2Signature_choice choice;
  union {
    dot2EcdsaP256Signature ecdsaNistP256Signature;
    dot2EcdsaP256Signature ecdsaBrainpoolP256r1Signature;
    dot2EcdsaP384Signature ecdsaBrainpoolP384r1Signature;
  } u;
} dot2Signature;

extern const ASN1CType asn1_type_dot2Signature[];

typedef enum dot2SymmAlgorithm {
  dot2SymmAlgorithm_aes128Ccm,
} dot2SymmAlgorithm;

extern const ASN1CType asn1_type_dot2SymmAlgorithm[];

typedef enum dot2HashAlgorithm {
  dot2HashAlgorithm_sha256,
  dot2HashAlgorithm_sha384,
} dot2HashAlgorithm;

extern const ASN1CType asn1_type_dot2HashAlgorithm[];

typedef struct dot2EciesP256EncryptedKey {
  dot2EccP256CurvePoint v;
  ASN1String c;
  ASN1String t;
} dot2EciesP256EncryptedKey;


extern const ASN1CType asn1_type_dot2EciesP256EncryptedKey[];

typedef enum {
  dot2BasePublicEncryptionKey_eciesNistP256,
  dot2BasePublicEncryptionKey_eciesBrainpoolP256r1,
} dot2BasePublicEncryptionKey_choice;

typedef struct dot2BasePublicEncryptionKey {
  dot2BasePublicEncryptionKey_choice choice;
  union {
    dot2EccP256CurvePoint eciesNistP256;
    dot2EccP256CurvePoint eciesBrainpoolP256r1;
  } u;
} dot2BasePublicEncryptionKey;

extern const ASN1CType asn1_type_dot2BasePublicEncryptionKey[];

typedef struct dot2PublicEncryptionKey {
  dot2SymmAlgorithm supportedSymmAlg;
  dot2BasePublicEncryptionKey publicKey;
} dot2PublicEncryptionKey;


extern const ASN1CType asn1_type_dot2PublicEncryptionKey[];

typedef enum {
  dot2SymmetricEncryptionKey_aes128Ccm,
} dot2SymmetricEncryptionKey_choice;

typedef struct dot2SymmetricEncryptionKey {
  dot2SymmetricEncryptionKey_choice choice;
  union {
    ASN1String aes128Ccm;
  } u;
} dot2SymmetricEncryptionKey;

extern const ASN1CType asn1_type_dot2SymmetricEncryptionKey[];

typedef enum {
  dot2EncryptionKey_Public,
  dot2EncryptionKey_symmetric,
} dot2EncryptionKey_choice;

typedef struct dot2EncryptionKey {
  dot2EncryptionKey_choice choice;
  union {
    dot2PublicEncryptionKey Public;
    dot2SymmetricEncryptionKey symmetric;
  } u;
} dot2EncryptionKey;

extern const ASN1CType asn1_type_dot2EncryptionKey[];

typedef enum {
  dot2PublicVerificationKey_ecdsaNistP256,
  dot2PublicVerificationKey_ecdsaBrainpoolP256r1,
  dot2PublicVerificationKey_ecdsaBrainpoolP384r1,
} dot2PublicVerificationKey_choice;

typedef struct dot2PublicVerificationKey {
  dot2PublicVerificationKey_choice choice;
  union {
    dot2EccP256CurvePoint ecdsaNistP256;
    dot2EccP256CurvePoint ecdsaBrainpoolP256r1;
    dot2EccP384CurvePoint ecdsaBrainpoolP384r1;
  } u;
} dot2PublicVerificationKey;

extern const ASN1CType asn1_type_dot2PublicVerificationKey[];

typedef ASN1Integer dot2Psid;

extern const ASN1CType asn1_type_dot2Psid[];

typedef ASN1String dot2BitmapSsp;

extern const ASN1CType asn1_type_dot2BitmapSsp[];

typedef enum {
  dot2ServiceSpecificPermissions_opaque,
  dot2ServiceSpecificPermissions_bitmapSsp,
} dot2ServiceSpecificPermissions_choice;

typedef struct dot2ServiceSpecificPermissions {
  dot2ServiceSpecificPermissions_choice choice;
  union {
    ASN1String opaque;
    dot2BitmapSsp bitmapSsp;
  } u;
} dot2ServiceSpecificPermissions;

extern const ASN1CType asn1_type_dot2ServiceSpecificPermissions[];

typedef struct dot2PsidSsp {
  dot2Psid psid;
  BOOL ssp_option;
  dot2ServiceSpecificPermissions ssp;
} dot2PsidSsp;


extern const ASN1CType asn1_type_dot2PsidSsp[];

typedef struct dot2SequenceOfPsidSsp {
  dot2PsidSsp *tab;
  size_t count;
} dot2SequenceOfPsidSsp;

extern const ASN1CType asn1_type_dot2SequenceOfPsidSsp[];

typedef struct dot2SequenceOfPsid {
  dot2Psid *tab;
  size_t count;
} dot2SequenceOfPsid;

extern const ASN1CType asn1_type_dot2SequenceOfPsid[];

typedef struct dot2SequenceOfOctetString {
  ASN1String *tab;
  size_t count;
} dot2SequenceOfOctetString;

extern const ASN1CType asn1_type_dot2SequenceOfOctetString[];

typedef struct dot2BitmapSspRange {
  ASN1String sspValue;
  ASN1String sspBitmask;
} dot2BitmapSspRange;


extern const ASN1CType asn1_type_dot2BitmapSspRange[];

typedef enum {
  dot2SspRange_opaque,
  dot2SspRange_all,
  dot2SspRange_bitmapSspRange,
} dot2SspRange_choice;

typedef struct dot2SspRange {
  dot2SspRange_choice choice;
  union {
    dot2SequenceOfOctetString opaque;
    dot2BitmapSspRange bitmapSspRange;
  } u;
} dot2SspRange;

extern const ASN1CType asn1_type_dot2SspRange[];

typedef struct dot2PsidSspRange {
  dot2Psid psid;
  BOOL sspRange_option;
  dot2SspRange sspRange;
} dot2PsidSspRange;


extern const ASN1CType asn1_type_dot2PsidSspRange[];

typedef struct dot2SequenceOfPsidSspRange {
  dot2PsidSspRange *tab;
  size_t count;
} dot2SequenceOfPsidSspRange;

extern const ASN1CType asn1_type_dot2SequenceOfPsidSspRange[];

typedef ASN1String dot2SubjectAssurance;

extern const ASN1CType asn1_type_dot2SubjectAssurance[];

typedef dot2Uint16 dot2CrlSeries;

#define asn1_type_dot2CrlSeries asn1_type_dot2Uint16

typedef dot2Uint16 dot2IValue;

extern const ASN1CType asn1_type_dot2IValue[];

typedef ASN1String dot2Hostname;

extern const ASN1CType asn1_type_dot2Hostname[];

typedef ASN1String dot2LinkageValue;

extern const ASN1CType asn1_type_dot2LinkageValue[];

typedef struct dot2GroupLinkageValue {
  ASN1String jValue;
  ASN1String value;
} dot2GroupLinkageValue;


extern const ASN1CType asn1_type_dot2GroupLinkageValue[];

typedef ASN1String dot2LaId;

extern const ASN1CType asn1_type_dot2LaId[];

typedef ASN1String dot2LinkageSeed;

extern const ASN1CType asn1_type_dot2LinkageSeed[];

typedef int dot2Uint8_1;

extern const ASN1CType asn1_type_dot2Uint8_1[];

typedef int dot2Uint8_3;

extern const ASN1CType asn1_type_dot2Uint8_3[];

typedef enum dot2CertificateType {
  dot2CertificateType_Explicit,
  dot2CertificateType_implicit,
} dot2CertificateType;

extern const ASN1CType asn1_type_dot2CertificateType[];

typedef enum {
  dot2IssuerIdentifier_sha256AndDigest,
  dot2IssuerIdentifier_self,
  dot2IssuerIdentifier_sha384AndDigest,
} dot2IssuerIdentifier_choice;

typedef struct dot2IssuerIdentifier {
  dot2IssuerIdentifier_choice choice;
  union {
    dot2HashedId8 sha256AndDigest;
    dot2HashAlgorithm self;
    dot2HashedId8 sha384AndDigest;
  } u;
} dot2IssuerIdentifier;

extern const ASN1CType asn1_type_dot2IssuerIdentifier[];

typedef struct dot2LinkageData {
  dot2IValue iCert;
  dot2LinkageValue linkage_value;
  BOOL group_linkage_value_option;
  dot2GroupLinkageValue group_linkage_value;
} dot2LinkageData;


extern const ASN1CType asn1_type_dot2LinkageData[];

typedef enum {
  dot2CertificateId_linkageData,
  dot2CertificateId_name,
  dot2CertificateId_binaryId,
  dot2CertificateId_none,
} dot2CertificateId_choice;

typedef struct dot2CertificateId {
  dot2CertificateId_choice choice;
  union {
    dot2LinkageData linkageData;
    dot2Hostname name;
    ASN1String binaryId;
  } u;
} dot2CertificateId;

extern const ASN1CType asn1_type_dot2CertificateId[];

typedef enum {
  dot2SubjectPermissions_Explicit,
  dot2SubjectPermissions_all,
} dot2SubjectPermissions_choice;

typedef struct dot2SubjectPermissions {
  dot2SubjectPermissions_choice choice;
  union {
    dot2SequenceOfPsidSspRange Explicit;
  } u;
} dot2SubjectPermissions;

extern const ASN1CType asn1_type_dot2SubjectPermissions[];

typedef ASN1BitString dot2EndEntityType;

extern const ASN1CType asn1_type_dot2EndEntityType[];

typedef struct dot2PsidGroupPermissions {
  dot2SubjectPermissions subjectPermissions;
  ASN1Integer minChainLength;
  ASN1Integer chainLengthRange;
  BOOL eeType_option;
  dot2EndEntityType eeType;
} dot2PsidGroupPermissions;


extern const ASN1CType asn1_type_dot2PsidGroupPermissions[];

typedef struct dot2SequenceOfPsidGroupPermissions {
  dot2PsidGroupPermissions *tab;
  size_t count;
} dot2SequenceOfPsidGroupPermissions;

extern const ASN1CType asn1_type_dot2SequenceOfPsidGroupPermissions[];

typedef enum {
  dot2VerificationKeyIndicator_verificationKey,
  dot2VerificationKeyIndicator_reconstructionValue,
} dot2VerificationKeyIndicator_choice;

typedef struct dot2VerificationKeyIndicator {
  dot2VerificationKeyIndicator_choice choice;
  union {
    dot2PublicVerificationKey verificationKey;
    dot2EccP256CurvePoint reconstructionValue;
  } u;
} dot2VerificationKeyIndicator;

extern const ASN1CType asn1_type_dot2VerificationKeyIndicator[];

typedef struct dot2ToBeSignedCertificate {
  dot2CertificateId id;
  dot2HashedId3 cracaId;
  dot2CrlSeries crlSeries;
  dot2ValidityPeriod validityPeriod;
  BOOL region_option;
  dot2GeographicRegion region;
  BOOL assuranceLevel_option;
  dot2SubjectAssurance assuranceLevel;
  BOOL appPermissions_option;
  dot2SequenceOfPsidSsp appPermissions;
  BOOL certIssuePermissions_option;
  dot2SequenceOfPsidGroupPermissions certIssuePermissions;
  BOOL certRequestPermissions_option;
  dot2SequenceOfPsidGroupPermissions certRequestPermissions;
  BOOL canRequestRollover_option;
  BOOL encryptionKey_option;
  dot2PublicEncryptionKey encryptionKey;
  dot2VerificationKeyIndicator verifyKeyIndicator;
} dot2ToBeSignedCertificate;


extern const ASN1CType asn1_type_dot2ToBeSignedCertificate[];

typedef struct dot2CertificateBase {
  dot2Uint8_3 version;
  dot2CertificateType type;
  dot2IssuerIdentifier issuer;
  dot2ToBeSignedCertificate toBeSigned;
  BOOL signature_option;
  dot2Signature signature;
} dot2CertificateBase;


extern const ASN1CType asn1_type_dot2CertificateBase[];

typedef dot2CertificateBase dot2Certificate;

#define asn1_type_dot2Certificate asn1_type_dot2CertificateBase

typedef struct dot2CaCertP2pPDU {
  dot2Certificate *tab;
  size_t count;
} dot2CaCertP2pPDU;

extern const ASN1CType asn1_type_dot2CaCertP2pPDU[];

typedef enum {
  dot2Ieee1609dot2Peer2PeerPDU_1_caCerts,
} dot2Ieee1609dot2Peer2PeerPDU_1_choice;

typedef struct dot2Ieee1609dot2Peer2PeerPDU_1 {
  dot2Ieee1609dot2Peer2PeerPDU_1_choice choice;
  union {
    dot2CaCertP2pPDU caCerts;
  } u;
} dot2Ieee1609dot2Peer2PeerPDU_1;

extern const ASN1CType asn1_type_dot2Ieee1609dot2Peer2PeerPDU_1[];

typedef struct dot2Ieee1609dot2Peer2PeerPDU {
  dot2Uint8_1 version;
  dot2Ieee1609dot2Peer2PeerPDU_1 content;
} dot2Ieee1609dot2Peer2PeerPDU;


extern const ASN1CType asn1_type_dot2Ieee1609dot2Peer2PeerPDU[];

typedef int dot2Uint8_2;

extern const ASN1CType asn1_type_dot2Uint8_2[];

typedef struct dot2MissingCrlIdentifier {
  dot2HashedId3 cracaId;
  dot2CrlSeries crlSeries;
} dot2MissingCrlIdentifier;


extern const ASN1CType asn1_type_dot2MissingCrlIdentifier[];

typedef struct dot2HeaderInfo {
  dot2Psid psid;
  BOOL generationTime_option;
  dot2Time64 generationTime;
  BOOL expiryTime_option;
  dot2Time64 expiryTime;
  BOOL generationLocation_option;
  dot2ThreeDLocation generationLocation;
  BOOL p2pcdLearningRequest_option;
  dot2HashedId3 p2pcdLearningRequest;
  BOOL missingCrlIdentifier_option;
  dot2MissingCrlIdentifier missingCrlIdentifier;
  BOOL encryptionKey_option;
  dot2EncryptionKey encryptionKey;
  BOOL inlineP2pcdRequest_option;
  dot2SequenceOfHashedId3 inlineP2pcdRequest;
  BOOL requestedCertificate_option;
  dot2Certificate requestedCertificate;
} dot2HeaderInfo;


extern const ASN1CType asn1_type_dot2HeaderInfo[];

typedef struct dot2ToBeSignedData {
  struct dot2SignedDataPayload * payload;
  dot2HeaderInfo headerInfo;
} dot2ToBeSignedData;


extern const ASN1CType asn1_type_dot2ToBeSignedData[];

typedef struct dot2SequenceOfCertificate {
  dot2Certificate *tab;
  size_t count;
} dot2SequenceOfCertificate;

extern const ASN1CType asn1_type_dot2SequenceOfCertificate[];

typedef enum {
  dot2SignerIdentifier_digest,
  dot2SignerIdentifier_certificate,
  dot2SignerIdentifier_self,
} dot2SignerIdentifier_choice;

typedef struct dot2SignerIdentifier {
  dot2SignerIdentifier_choice choice;
  union {
    dot2HashedId8 digest;
    dot2SequenceOfCertificate certificate;
  } u;
} dot2SignerIdentifier;

extern const ASN1CType asn1_type_dot2SignerIdentifier[];

typedef struct dot2SignedData {
  dot2HashAlgorithm hashId;
  dot2ToBeSignedData tbsData;
  dot2SignerIdentifier signer;
  dot2Signature signature;
} dot2SignedData;


extern const ASN1CType asn1_type_dot2SignedData[];

typedef dot2HashedId8 dot2PreSharedKeyRecipientInfo;

extern const ASN1CType asn1_type_dot2PreSharedKeyRecipientInfo[];

typedef struct dot2AesCcmCiphertext {
  ASN1String nonce;
  dot2Opaque ccmCiphertext;
} dot2AesCcmCiphertext;


extern const ASN1CType asn1_type_dot2AesCcmCiphertext[];

typedef enum {
  dot2SymmetricCiphertext_aes128ccm,
} dot2SymmetricCiphertext_choice;

typedef struct dot2SymmetricCiphertext {
  dot2SymmetricCiphertext_choice choice;
  union {
    dot2AesCcmCiphertext aes128ccm;
  } u;
} dot2SymmetricCiphertext;

extern const ASN1CType asn1_type_dot2SymmetricCiphertext[];

typedef struct dot2SymmRecipientInfo {
  dot2HashedId8 recipientId;
  dot2SymmetricCiphertext encKey;
} dot2SymmRecipientInfo;


extern const ASN1CType asn1_type_dot2SymmRecipientInfo[];

typedef enum {
  dot2EncryptedDataEncryptionKey_eciesNistP256,
  dot2EncryptedDataEncryptionKey_eciesBrainpoolP256r1,
} dot2EncryptedDataEncryptionKey_choice;

typedef struct dot2EncryptedDataEncryptionKey {
  dot2EncryptedDataEncryptionKey_choice choice;
  union {
    dot2EciesP256EncryptedKey eciesNistP256;
    dot2EciesP256EncryptedKey eciesBrainpoolP256r1;
  } u;
} dot2EncryptedDataEncryptionKey;

extern const ASN1CType asn1_type_dot2EncryptedDataEncryptionKey[];

typedef struct dot2PKRecipientInfo {
  dot2HashedId8 recipientId;
  dot2EncryptedDataEncryptionKey encKey;
} dot2PKRecipientInfo;


extern const ASN1CType asn1_type_dot2PKRecipientInfo[];

typedef enum {
  dot2RecipientInfo_pskRecipInfo,
  dot2RecipientInfo_symmRecipInfo,
  dot2RecipientInfo_certRecipInfo,
  dot2RecipientInfo_signedDataRecipInfo,
  dot2RecipientInfo_rekRecipInfo,
} dot2RecipientInfo_choice;

typedef struct dot2RecipientInfo {
  dot2RecipientInfo_choice choice;
  union {
    dot2PreSharedKeyRecipientInfo pskRecipInfo;
    dot2SymmRecipientInfo symmRecipInfo;
    dot2PKRecipientInfo certRecipInfo;
    dot2PKRecipientInfo signedDataRecipInfo;
    dot2PKRecipientInfo rekRecipInfo;
  } u;
} dot2RecipientInfo;

extern const ASN1CType asn1_type_dot2RecipientInfo[];

typedef struct dot2SequenceOfRecipientInfo {
  dot2RecipientInfo *tab;
  size_t count;
} dot2SequenceOfRecipientInfo;

extern const ASN1CType asn1_type_dot2SequenceOfRecipientInfo[];

typedef struct dot2EncryptedData {
  dot2SequenceOfRecipientInfo recipients;
  dot2SymmetricCiphertext ciphertext;
} dot2EncryptedData;


extern const ASN1CType asn1_type_dot2EncryptedData[];

typedef enum {
  dot2Ieee1609Dot2Content_unsecuredData,
  dot2Ieee1609Dot2Content_signedData,
  dot2Ieee1609Dot2Content_encryptedData,
  dot2Ieee1609Dot2Content_signedCertificateRequest,
} dot2Ieee1609Dot2Content_choice;

typedef struct dot2Ieee1609Dot2Content {
  dot2Ieee1609Dot2Content_choice choice;
  union {
    dot2Opaque unsecuredData;
    dot2SignedData signedData;
    dot2EncryptedData encryptedData;
    dot2Opaque signedCertificateRequest;
  } u;
} dot2Ieee1609Dot2Content;

extern const ASN1CType asn1_type_dot2Ieee1609Dot2Content[];

typedef struct dot2Ieee1609Dot2Data {
  dot2Uint8_2 protocolVersion;
  dot2Ieee1609Dot2Content content;
} dot2Ieee1609Dot2Data;


extern const ASN1CType asn1_type_dot2Ieee1609Dot2Data[];

typedef enum {
  dot2HashedData_sha256HashedData,
} dot2HashedData_choice;

typedef struct dot2HashedData {
  dot2HashedData_choice choice;
  union {
    ASN1String sha256HashedData;
  } u;
} dot2HashedData;

extern const ASN1CType asn1_type_dot2HashedData[];

typedef struct dot2SignedDataPayload {
  BOOL data_option;
  dot2Ieee1609Dot2Data data;
  BOOL extDataHash_option;
  dot2HashedData extDataHash;
} dot2SignedDataPayload;


extern const ASN1CType asn1_type_dot2SignedDataPayload[];

typedef dot2Ieee1609Dot2Data dot2Countersignature;

#define asn1_type_dot2Countersignature asn1_type_dot2Ieee1609Dot2Data

typedef dot2CertificateBase dot2ImplicitCertificate;

#define asn1_type_dot2ImplicitCertificate asn1_type_dot2CertificateBase

typedef dot2CertificateBase dot2ExplicitCertificate;

#define asn1_type_dot2ExplicitCertificate asn1_type_dot2CertificateBase

typedef int dot2Uint8_4;

extern const ASN1CType asn1_type_dot2Uint8_4[];

typedef struct dot2CrlPriorityInfo {
  BOOL priority_option;
  dot2Uint8 priority;
} dot2CrlPriorityInfo;


extern const ASN1CType asn1_type_dot2CrlPriorityInfo[];

typedef struct dot2HashBasedRevocationInfo {
  dot2HashedId10 id;
  dot2Time32 expiry;
} dot2HashBasedRevocationInfo;


extern const ASN1CType asn1_type_dot2HashBasedRevocationInfo[];

typedef struct dot2SequenceOfHashBasedRevocationInfo {
  dot2HashBasedRevocationInfo *tab;
  size_t count;
} dot2SequenceOfHashBasedRevocationInfo;

extern const ASN1CType asn1_type_dot2SequenceOfHashBasedRevocationInfo[];

typedef struct dot2ToBeSignedHashIdCrl {
  dot2Uint32 crlSerial;
  dot2SequenceOfHashBasedRevocationInfo entries;
} dot2ToBeSignedHashIdCrl;


extern const ASN1CType asn1_type_dot2ToBeSignedHashIdCrl[];

typedef struct dot2IndividualRevocation {
  dot2LinkageSeed linkage_seed1;
  dot2LinkageSeed linkage_seed2;
} dot2IndividualRevocation;


extern const ASN1CType asn1_type_dot2IndividualRevocation[];

typedef struct dot2SequenceOfIndividualRevocation {
  dot2IndividualRevocation *tab;
  size_t count;
} dot2SequenceOfIndividualRevocation;

extern const ASN1CType asn1_type_dot2SequenceOfIndividualRevocation[];

typedef struct dot2IMaxGroup {
  dot2Uint16 iMax;
  dot2SequenceOfIndividualRevocation contents;
} dot2IMaxGroup;


extern const ASN1CType asn1_type_dot2IMaxGroup[];

typedef struct dot2SequenceOfIMaxGroup {
  dot2IMaxGroup *tab;
  size_t count;
} dot2SequenceOfIMaxGroup;

extern const ASN1CType asn1_type_dot2SequenceOfIMaxGroup[];

typedef struct dot2LAGroup {
  dot2LaId la1Id;
  dot2LaId la2Id;
  dot2SequenceOfIMaxGroup contents;
} dot2LAGroup;


extern const ASN1CType asn1_type_dot2LAGroup[];

typedef struct dot2SequenceOfLAGroup {
  dot2LAGroup *tab;
  size_t count;
} dot2SequenceOfLAGroup;

extern const ASN1CType asn1_type_dot2SequenceOfLAGroup[];

typedef struct dot2JMaxGroup {
  dot2Uint8 jmax;
  dot2SequenceOfLAGroup contents;
} dot2JMaxGroup;


extern const ASN1CType asn1_type_dot2JMaxGroup[];

typedef struct dot2SequenceOfJMaxGroup {
  dot2JMaxGroup *tab;
  size_t count;
} dot2SequenceOfJMaxGroup;

extern const ASN1CType asn1_type_dot2SequenceOfJMaxGroup[];

typedef struct dot2GroupCrlEntry {
  dot2Uint16 iMax;
  dot2LaId la1Id;
  dot2LinkageSeed linkageSeed1;
  dot2LaId la2Id;
  dot2LinkageSeed linkageSeed2;
} dot2GroupCrlEntry;


extern const ASN1CType asn1_type_dot2GroupCrlEntry[];

typedef struct dot2SequenceOfGroupCrlEntry {
  dot2GroupCrlEntry *tab;
  size_t count;
} dot2SequenceOfGroupCrlEntry;

extern const ASN1CType asn1_type_dot2SequenceOfGroupCrlEntry[];

typedef struct dot2ToBeSignedLinkageValueCrl {
  dot2IValue iRev;
  dot2Uint8 indexWithinI;
  BOOL individual_option;
  dot2SequenceOfJMaxGroup individual;
  BOOL groups_option;
  dot2SequenceOfGroupCrlEntry groups;
} dot2ToBeSignedLinkageValueCrl;


extern const ASN1CType asn1_type_dot2ToBeSignedLinkageValueCrl[];

typedef enum {
  dot2CrlContents_1_fullHashCrl,
  dot2CrlContents_1_deltaHashCrl,
  dot2CrlContents_1_fullLinkedCrl,
  dot2CrlContents_1_deltaLinkedCrl,
} dot2CrlContents_1_choice;

typedef struct dot2CrlContents_1 {
  dot2CrlContents_1_choice choice;
  union {
    dot2ToBeSignedHashIdCrl fullHashCrl;
    dot2ToBeSignedHashIdCrl deltaHashCrl;
    dot2ToBeSignedLinkageValueCrl fullLinkedCrl;
    dot2ToBeSignedLinkageValueCrl deltaLinkedCrl;
  } u;
} dot2CrlContents_1;

extern const ASN1CType asn1_type_dot2CrlContents_1[];

typedef struct dot2CrlContents {
  dot2Uint8_4 version;
  dot2CrlSeries crlSeries;
  dot2HashedId8 cracaId;
  dot2Time32 issueDate;
  dot2Time32 nextCrl;
  dot2CrlPriorityInfo priorityInfo;
  dot2CrlContents_1 typeSpecific;
} dot2CrlContents;


extern const ASN1CType asn1_type_dot2CrlContents[];

typedef int dot2Psid_1;

extern const ASN1CType asn1_type_dot2Psid_1[];

typedef dot2Psid_1 dot2CrlPsid;

#define asn1_type_dot2CrlPsid asn1_type_dot2Psid_1

typedef dot2Ieee1609Dot2Data dot2SecuredCrl;

#define asn1_type_dot2SecuredCrl asn1_type_dot2Ieee1609Dot2Data

typedef int dot2Uint8_5;

extern const ASN1CType asn1_type_dot2Uint8_5[];

typedef enum dot2CracaType {
  dot2CracaType_isCraca,
  dot2CracaType_issuerIsCraca,
} dot2CracaType;

extern const ASN1CType asn1_type_dot2CracaType[];

typedef struct dot2PermissibleCrls {
  dot2CrlSeries *tab;
  size_t count;
} dot2PermissibleCrls;

extern const ASN1CType asn1_type_dot2PermissibleCrls[];

typedef struct dot2CrlSsp {
  dot2Uint8_5 version;
  dot2CracaType associatedCraca;
  dot2PermissibleCrls crls;
} dot2CrlSsp;


extern const ASN1CType asn1_type_dot2CrlSsp[];

#ifdef  __cplusplus
}
#endif

#endif /* _FFASN1_FFASN1_DOT2_2016_H */
