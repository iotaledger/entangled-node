{
  "targets": [
    {
      "target_name": "entangled",
      "sources": [
         "src/interface.cpp",
         "entangled/common/helpers/pow.c",
         "entangled/common/pow/pow.c",
         "entangled/common/helpers/sign.c",
         "entangled/common/trinary/trit_ptrit.c",
         "entangled/common/trinary/ptrit_incr.c",
         "entangled/common/trinary/trit_tryte.c",
         "entangled/common/trinary/add.c",
         "entangled/common/kerl/kerl.c",
         "entangled/common/kerl/converter.c",
         "entangled/common/kerl/bigint.c",
         "entangled/common/sign/v1/iss_curl.c",
         "entangled/common/sign/v1/iss_kerl.c",
         "entangled/common/curl-p/const.c",
         "entangled/common/curl-p/curl_p.c",
         "entangled/common/curl-p/ptrit.c",
         "entangled/common/curl-p/hashcash.c",
         "entangled/common/curl-p/pearl_diver.c",
         "entangled/utils/system.c",
         "src/keccak/KeccakHash.c",
         "src/keccak/KeccakSponge.c",
         "src/keccak/KeccakF-1600-reference.c",
      ],
      "cflags+": ["-std=gnu99"],
      "include_dirs" : [
         "<!(node -e \"require('nan')\")",
         "entangled",
         "src",
         "src/utarray"
      ]
    }
  ]
}
