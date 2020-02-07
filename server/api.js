const express = require("express");

const router = express.Router();
var cecko = require("./algorithms_c++/build/Release/server");

router.post("/grammarRequest/reducedNormalForm", (req, res) => {
  const { grammar } = req.body;
  res.send(cecko.execute("reducedNormalForm", grammar));
});

router.post("/grammarRequest/epsilonFreeForm", (req, res) => {
  const { grammar } = req.body;
  res.send(cecko.execute("epsilonFreeForm", grammar));
});

router.post("/grammarRequest/equivalence", (req, res) => {
  const { grammar1 } = req.body;
  const { grammar2 } = req.body;

  res.send(cecko.execute("equivalence", grammar1, grammar2));
});

router.post("/grammarRequest/isPrecedential", (req, res) => {
  const { grammar } = req.body;

  res.send(cecko.execute("isPrecedential", grammar));
});

router.post("/grammarRequest/getWordSyntaxAnalysis", (req, res) => {
  const { grammar } = req.body;
  const { word } = req.body;

  res.send(cecko.execute("getWordSyntaxAnalysis", grammar, word));
});

module.exports = {
  router: router
};
