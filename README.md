# Entangled node
[![Build Status](https://travis-ci.org/rihardsgravis/entangled-node.svg?branch=master)](https://travis-ci.org/rihardsgravis/entangled-node)

[IOTA Common](https://github.com/iotaledger/iota_common) native Node.js module bindings, exposing functions for Proof of Work, address generation, signature generation and bundle mining.

Usage:

```javascript
const {
  powTrytesFunc,
  powBundleFunc,
  genAddressTrytesFunc,
  genAddressTritsFunc,
  genSignatureTrytesFunc,
  genSignatureTritsFunc,
  transactionHashFunc,
  bundleMiner } = require('entangled-node');

(async () => {

  // Do Proof of Work on trytes
  const pow = await powTrytesFunc("TRYTES", 14);

  // Do Proof of Work on a bundle
  const transactions = await powBundleFunc({"TRYTES1", "TRYTES2", "TRYTES3"}, "TRUNK", "BRANCH", 14);

  // Generate address trytes
  const addressTrytes = await genAddressTrytesFunc("SEED", 0, 2);

  // Generate address trits
  const addressTrits = await genAddressTritsFunc([-1, 0, ..., 1, 1], 0, 2);

  // Generate signature trytes
  const signatureTrytes = await genSignatureTrytesFunc("SEED", 0, 2, "BUNDLEHASH");

  // Generate signature trits
  const signatureTrits = await genSignatureTritsFunc([1, 0, ..., 0, -1], 0, 2, [-1, -1, ..., 1, 0]);

  // Transaction hash
  const hash = await transactionHashFunc("TRYTES");

  // Bundle miner
  const index = await bundleMiner([-6, 4, ..., 8, -9], 2, [0, -1, ..., 1, 1], 486 * 4, 1000000, 0);

})();
```
