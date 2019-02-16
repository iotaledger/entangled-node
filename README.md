# Entangled node
[![Build Status](https://travis-ci.org/rihardsgravis/entangled-node.svg?branch=master)](https://travis-ci.org/rihardsgravis/entangled-node)

[IOTA Entangled](https://github.com/iotaledger/entangled) native Node.js module bindings, exposing functions for Proof of Work,  address generation and signature generation.

Usage:

```javascript
const {
  powTrytesFunc,
  powBundleFunc,
  genAddressTrytesFunc,
  genAddressTritsFunc,
  genSignatureTrytesFunc,
  genSignatureTritsFunc,
  transactionHashFunc } = require('entangled-node');

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
  const hash = await transactionHash("TRYTES");

})();
