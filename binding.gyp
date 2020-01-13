{
  "targets": [
    {
      "target_name": "iota_common",
      "sources": [
         "src/interface.cpp",
         "iota_common/common/model/bundle.c",
         "iota_common/common/model/transaction.c",
         "iota_common/common/helpers/pow.c",
         "iota_common/common/helpers/sign.c",
         "iota_common/common/helpers/digest.c",
         "iota_common/common/trinary/trit_long.c",
         "iota_common/common/trinary/trit_byte.c",
         "iota_common/common/trinary/ptrit_incr.c",
         "iota_common/common/trinary/trit_tryte.c",
         "iota_common/common/trinary/flex_trit.c",
         "iota_common/common/trinary/add.c",
         "iota_common/common/trinary/ptrit.c",
         "iota_common/common/crypto/kerl/kerl.c",
         "iota_common/common/crypto/kerl/converter.c",
         "iota_common/common/crypto/kerl/bigint.c",
         "iota_common/common/crypto/iss/v1/iss_curl.c",
         "iota_common/common/crypto/iss/v1/iss_kerl.c",
         "iota_common/common/crypto/iss/normalize.c",
         "iota_common/common/crypto/curl-p/digest.c",
         "iota_common/common/crypto/curl-p/const.c",
         "iota_common/common/crypto/curl-p/curl_p.c",
         "iota_common/common/crypto/curl-p/ptrit.c",
         "iota_common/common/crypto/curl-p/hashcash.c",
         "iota_common/common/crypto/curl-p/pearl_diver.c",
         "iota_common/utils/bundle_miner.c",
         "iota_common/utils/memset_safe.c",
         "iota_common/utils/system.c",
         "iota_common/utils/time.c",
         "src/keccak/KeccakP-1600-reference.c",
         "src/keccak/KeccakSpongeWidth1600.c",
         "src/keccak/KeccakHash.c",
      ],
      "cflags+": ["-std=gnu99", "-msse2"],
      "conditions": [
          ['OS=="mac"', {
            "xcode_settings": {
              "GCC_C_LANGUAGE_STANDARD": "gnu99",
              "OTHER_CFLAGS" : ["-msse2"],
            },
          }],
        ],
      "include_dirs": [
         "<!(node -e \"require('nan')\")",
         "iota_common",
         "src",
         "src/utarray",
         "src/embear"
      ],
      "defines": [
        "PCURL_STATE_SHORT",
        "PCURL_SBOX_UNWIND_4",
        "PTRIT_SSE2"
      ]
    }
  ]
}
