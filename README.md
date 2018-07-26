# Entangled node
[![Build Status](https://travis-ci.org/rihardsgravis/entangled-node.svg?branch=master)](https://travis-ci.org/rihardsgravis/entangled-node)

[IOTA Entangled](https://github.com/iotaledger/entangled) native Node.js module bindings, exposing functions for Proof of Work and address generation.

Usage:

```javascript
const { powFunc, genFunc } = require('entangled-node');

(async () => {
  
  // Do Proof of Work
  const pow = await powFunc("TRYTES", 14);
  
  // Generate address
  const address = await genFunc("SEED", 0, 2)

})();
