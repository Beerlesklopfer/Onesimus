// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file btlstestcerts.h
 * @brief TEST-ONLY certificates and keys for the TLS tests (P0-6, P0-7, P0-11)
 *
 * Generated once with openssl (EC P-256, valid for 100 years). The keys are
 * public on purpose and protect nothing: never use them outside tests.
 *
 * - Ca: test CA
 * - Local: signed by Ca, CN localhost, SAN IP:127.0.0.1, DNS:localhost
 * - Other: signed by Ca, CN other.example, SAN DNS:other.example
 * - DirName: signed by Ca, CN bareos-dir, no SAN (Bareos style daemon name)
 * - PinA, PinB: two self-signed certificates like the one odir generates
 */

#ifndef BTLSTESTCERTS_H
#define BTLSTESTCERTS_H

namespace BTlsTestCerts {

inline const char CaCert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBnTCCAUOgAwIBAgIUeTYDnv2N2pry7gT+HqzupxlREjwwCgYIKoZIzj0EAwIw\n"
    "GzEZMBcGA1UEAwwQT25lc2ltdXMgVGVzdCBDQTAgFw0yNjExMjMwODIxMjhaGA8y\n"
    "MTI2MTAzMDA4MjEyOFowGzEZMBcGA1UEAwwQT25lc2ltdXMgVGVzdCBDQTBZMBMG\n"
    "ByqGSM49AgEGCCqGSM49AwEHA0IABB39UF5gs3kMT16Iz076wIFSw2Usje0NJ0NL\n"
    "fkYDMrtpoPWYozI1OunqHsPp2IlyDPpodio8357uNM7ZT/PpPV6jYzBhMB0GA1Ud\n"
    "DgQWBBTIJn2vX3PDe+12uFaQdM5HCXXsajAfBgNVHSMEGDAWgBTIJn2vX3PDe+12\n"
    "uFaQdM5HCXXsajAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBBjAKBggq\n"
    "hkjOPQQDAgNIADBFAiEAmkfUMjbaVdLFUM+TzCtcGqj5LLH0gBA9kKHpyVMLihoC\n"
    "IFt/69P7xc8qwqfutrJTtbEtfpUtLKD1LCUZUAkuTM0H\n"
    "-----END CERTIFICATE-----\n";

inline const char LocalCert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIByzCCAXCgAwIBAgIUMWm/Z8OJMcdx3aRksFNk122lqEYwCgYIKoZIzj0EAwIw\n"
    "GzEZMBcGA1UEAwwQT25lc2ltdXMgVGVzdCBDQTAgFw0yNjExMjMwODIxMjhaGA8y\n"
    "MTI2MTAzMDA4MjEyOFowFDESMBAGA1UEAwwJbG9jYWxob3N0MFkwEwYHKoZIzj0C\n"
    "AQYIKoZIzj0DAQcDQgAEku8STBXjUvQf0cI1coRjYvu96xbZfWEcN9zTBrv33mG/\n"
    "gKCYf80OLyoA/NwdgkrXd91C1SLMIyXcJszlfXRS76OBljCBkzAJBgNVHRMEAjAA\n"
    "MAsGA1UdDwQEAwIHgDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwGgYD\n"
    "VR0RBBMwEYcEfwAAAYIJbG9jYWxob3N0MB0GA1UdDgQWBBTMkiyG767w97AynHO0\n"
    "ymzql8B4bjAfBgNVHSMEGDAWgBTIJn2vX3PDe+12uFaQdM5HCXXsajAKBggqhkjO\n"
    "PQQDAgNJADBGAiEA7eZBAxp4s2Ynm0CdOtiN9rR4Cx1PJX9GOpI9xlmk198CIQCh\n"
    "Q/NHX2Vjr0BT5YxlCRh1NMq9lOSV3FaqDOcVIv2new==\n"
    "-----END CERTIFICATE-----\n";

inline const char LocalKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgEfLTu+NE6BbobTf/\n"
    "LzJjNdRtc9AiCMqmMmb6fy92hWyhRANCAASS7xJMFeNS9B/RwjVyhGNi+73rFtl9\n"
    "YRw33NMGu/feYb+AoJh/zQ4vKgD83B2CStd33ULVIswjJdwmzOV9dFLv\n"
    "-----END PRIVATE KEY-----\n";

inline const char OtherCert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBzDCCAXKgAwIBAgIUMWm/Z8OJMcdx3aRksFNk122lqEcwCgYIKoZIzj0EAwIw\n"
    "GzEZMBcGA1UEAwwQT25lc2ltdXMgVGVzdCBDQTAgFw0yNjExMjMwODIxMjhaGA8y\n"
    "MTI2MTAzMDA4MjEyOFowGDEWMBQGA1UEAwwNb3RoZXIuZXhhbXBsZTBZMBMGByqG\n"
    "SM49AgEGCCqGSM49AwEHA0IABH2IhkR9BgpYKzHUDs5RewELA6a24rG/GBmLVz/b\n"
    "kBEtKjtcYXZmkBg9YXeEcN2keIT4wUc+Pu3mbrARGEby4FyjgZQwgZEwCQYDVR0T\n"
    "BAIwADALBgNVHQ8EBAMCB4AwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMC\n"
    "MBgGA1UdEQQRMA+CDW90aGVyLmV4YW1wbGUwHQYDVR0OBBYEFCi7exkg5e8x3S1g\n"
    "lrCImXOyLUQ9MB8GA1UdIwQYMBaAFMgmfa9fc8N77Xa4VpB0zkcJdexqMAoGCCqG\n"
    "SM49BAMCA0gAMEUCIQCwUaiGt1jYbkkNl8bz1ZiGzcjNlH3bQiKFr85B8CA2SQIg\n"
    "e0Q2Anj7sxjpKUtBp5a27VmL2Mr3fEPi107bSOMd0rc=\n"
    "-----END CERTIFICATE-----\n";

inline const char OtherKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQguZ+4EQUd1eMXEVmR\n"
    "kU1Y7ogi//A9NSj9HFUbCRSFgg6hRANCAAR9iIZEfQYKWCsx1A7OUXsBCwOmtuKx\n"
    "vxgZi1c/25ARLSo7XGF2ZpAYPWF3hHDdpHiE+MFHPj7t5m6wERhG8uBc\n"
    "-----END PRIVATE KEY-----\n";

inline const char DirNameCert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBrjCCAVOgAwIBAgIUMWm/Z8OJMcdx3aRksFNk122lqEgwCgYIKoZIzj0EAwIw\n"
    "GzEZMBcGA1UEAwwQT25lc2ltdXMgVGVzdCBDQTAgFw0yNjExMjMwODIxMjlaGA8y\n"
    "MTI2MTAzMDA4MjEyOVowFTETMBEGA1UEAwwKYmFyZW9zLWRpcjBZMBMGByqGSM49\n"
    "AgEGCCqGSM49AwEHA0IABDVV42Zopy0DUmQeO1oVy2Jc0DeVVHC4nQ+0Cu6gheVp\n"
    "Oz69uBWURGznaYWxeC7aOEHN+7GdpdZi6oWnphOoRxCjeTB3MAkGA1UdEwQCMAAw\n"
    "CwYDVR0PBAQDAgeAMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAdBgNV\n"
    "HQ4EFgQU26NsY5r9RYVlWMvfSfqY7r5mE+UwHwYDVR0jBBgwFoAUyCZ9r19zw3vt\n"
    "drhWkHTORwl17GowCgYIKoZIzj0EAwIDSQAwRgIhALwpsY3/OzkVxR3idZV9UmRp\n"
    "UxDiC86/x9DYxduKU2LCAiEAiOCej+JyM6mS2Y2CPHby6uDm9MjfcR/34fyNIclv\n"
    "Jjk=\n"
    "-----END CERTIFICATE-----\n";

inline const char DirNameKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg6N5XZ02OeQ9THaPK\n"
    "W2BTkhwLLZAe3XqsGdTjjqjSKdqhRANCAAQ1VeNmaKctA1JkHjtaFctiXNA3lVRw\n"
    "uJ0PtAruoIXlaTs+vbgVlERs52mFsXgu2jhBzfuxnaXWYuqFp6YTqEcQ\n"
    "-----END PRIVATE KEY-----\n";

inline const char PinACert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBnDCCAUOgAwIBAgIUK2Pr6BX9mTdvZj5p+TIEQSryqAcwCgYIKoZIzj0EAwIw\n"
    "EzERMA8GA1UEAwwIdGVzdC1kaXIwIBcNMjYxMTIzMDgyMTI5WhgPMjEyNjEwMzAw\n"
    "ODIxMjlaMBMxETAPBgNVBAMMCHRlc3QtZGlyMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
    "AQcDQgAEWwy7UD1UEv7/nVqKyOI4ebbNAiggcZyV7o2tCor4/Nx2TVh524crflOz\n"
    "hjo7FoDV8TLfr7ZQ4OVdCSQG2cQMEKNzMHEwHQYDVR0OBBYEFPertK4/RifqJZyx\n"
    "Gd1q3nlF40jCMB8GA1UdIwQYMBaAFPertK4/RifqJZyxGd1q3nlF40jCMA8GA1Ud\n"
    "EwEB/wQFMAMBAf8wHgYDVR0RBBcwFYIIdGVzdC1kaXKCCWxvY2FsaG9zdDAKBggq\n"
    "hkjOPQQDAgNHADBEAiBBv5b5xGx/RNgrh5LJ8GByKx8nFDAfthbGvP3yFr5POwIg\n"
    "Zzw98VLHTIkZ118cTyG4hO+pscAqnTFUYzdRoTjKiF0=\n"
    "-----END CERTIFICATE-----\n";

inline const char PinAKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgCi2BRw4STejw718M\n"
    "wV3KKm31n4DhqSYoPlvDU2yNu92hRANCAARbDLtQPVQS/v+dWorI4jh5ts0CKCBx\n"
    "nJXuja0Kivj83HZNWHnbhyt+U7OGOjsWgNXxMt+vtlDg5V0JJAbZxAwQ\n"
    "-----END PRIVATE KEY-----\n";

inline const char PinBCert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBnTCCAUOgAwIBAgIUJAGMJvk+M6t1TVAcXtgo5HeULEYwCgYIKoZIzj0EAwIw\n"
    "EzERMA8GA1UEAwwIdGVzdC1kaXIwIBcNMjYxMTIzMDgyMTI5WhgPMjEyNjEwMzAw\n"
    "ODIxMjlaMBMxETAPBgNVBAMMCHRlc3QtZGlyMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
    "AQcDQgAE82aqteVCwhJW3Zl7l2CE63JjnV96Tf00MregQ3CDuQFpXGQG2z+lmoKh\n"
    "aQkth9aoYmGjod4rKA8MaZS0LcyyRKNzMHEwHQYDVR0OBBYEFJh6wNWtrnHAtESR\n"
    "LqsxGEPdMbfBMB8GA1UdIwQYMBaAFJh6wNWtrnHAtESRLqsxGEPdMbfBMA8GA1Ud\n"
    "EwEB/wQFMAMBAf8wHgYDVR0RBBcwFYIIdGVzdC1kaXKCCWxvY2FsaG9zdDAKBggq\n"
    "hkjOPQQDAgNIADBFAiEAq1TJ/k+7RfSGTUAUO5WvvGE+CRd+SSsEpkfKV1NrXGoC\n"
    "IGdDMcYaH9v3hz85y2nFUTwEeS6AlY7hnPboJzs4w9px\n"
    "-----END CERTIFICATE-----\n";

inline const char PinBKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgf7vATtAJlSPXHvYq\n"
    "Jws+4Q4mtxc6uVmEwMDOx9sPMzChRANCAATzZqq15ULCElbdmXuXYITrcmOdX3pN\n"
    "/TQyt6BDcIO5AWlcZAbbP6WagqFpCS2H1qhiYaOh3isoDwxplLQtzLJE\n"
    "-----END PRIVATE KEY-----\n";

} // namespace BTlsTestCerts

#endif // BTLSTESTCERTS_H
