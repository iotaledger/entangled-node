{
  "targets": [
    {
      "target_name": "entangled",
      "sources": [
         "src/interface.cpp",
         "entangled/common/model/bundle.c",
         "entangled/common/model/transaction.c",
         "entangled/common/helpers/pow.c",
         "entangled/common/helpers/sign.c",
         "entangled/common/helpers/digest.c",
         "entangled/common/trinary/trit_long.c",
         "entangled/common/trinary/trit_byte.c",
         "entangled/common/trinary/ptrit_incr.c",
         "entangled/common/trinary/trit_tryte.c",
         "entangled/common/trinary/flex_trit.c",
         "entangled/common/trinary/add.c",
         "entangled/common/trinary/ptrit.c",
         "entangled/common/crypto/kerl/kerl.c",
         "entangled/common/crypto/kerl/converter.c",
         "entangled/common/crypto/kerl/bigint.c",
         "entangled/common/crypto/iss/v1/iss_curl.c",
         "entangled/common/crypto/iss/v1/iss_kerl.c",
         "entangled/common/crypto/iss/normalize.c",
         "entangled/common/crypto/curl-p/digest.c",
         "entangled/common/crypto/curl-p/const.c",
         "entangled/common/crypto/curl-p/curl_p.c",
         "entangled/common/crypto/curl-p/ptrit.c",
         "entangled/common/crypto/curl-p/hashcash.c",
         "entangled/common/crypto/curl-p/pearl_diver.c",
         "entangled/utils/bundle_miner.c",
         "entangled/utils/memset_safe.c",
         "entangled/utils/system.c",
         "entangled/utils/time.c",
         "src/keccak/KeccakHash.c",
         "src/keccak/KeccakSponge.c",
         "src/keccak/KeccakF-1600-reference.c",
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
         "entangled",
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
