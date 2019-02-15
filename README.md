# Entangled node
[![Build Status](https://travis-ci.org/rihardsgravis/entangled-node.svg?branch=master)](https://travis-ci.org/rihardsgravis/entangled-node)

[IOTA Entangled](https://github.com/iotaledger/entangled) native Node.js module bindings, exposing functions for Proof of Work,  address generation and signature generation.

Usage:

```javascript
const { powTrytesFunc, genAddressTrytesFunc } = require('entangled-node');

(async () => {

  // Do Proof of Work
  const pow = await powTrytesFunc("TRYTES", 14);

  // Generate address trytes
  const address = await genAddressTrytesFunc("SEED", 0, 2)

})();
