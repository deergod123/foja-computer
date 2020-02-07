import { epsilon, substitutedEpsilon } from "./Constants";

export const stringToGrammar = stringGrammar => {
  let grammar = stringGrammar.split("|");
  let nonterminalsSet = [];
  let terminalsSet = [];
  let start = "";
  let rulesSet = [];

  if (grammar.length === 0)
    return { nonterminalsSet, terminalsSet, start, rulesSet };

  nonterminalsSet = grammar[0].split(",");

  if (grammar.length === 1)
    return { nonterminalsSet, terminalsSet, start, rulesSet };

  terminalsSet = grammar[1].split(",");

  if (grammar.length === 2)
    return { nonterminalsSet, terminalsSet, start, rulesSet };

  start = grammar[2];

  if (grammar.length === 3)
    return { nonterminalsSet, terminalsSet, start, rulesSet };

  let rulesSetArray = grammar[3].split(",");

  for (let i = 0; i < rulesSetArray.length; i += 2) {
    if (rulesSetArray[i + 1] === substitutedEpsilon) {
      rulesSet.push([rulesSetArray[i], epsilon]);
    } else {
      rulesSet.push([rulesSetArray[i], rulesSetArray[i + 1]]);
    }
  }
  return { nonterminalsSet, terminalsSet, start, rulesSet };
};

export const grammarToString = grammar => {
  let stringGrammar = "";
  for (let i = 0; i < grammar.nonterminalsSet.length; i += 1) {
    stringGrammar += grammar.nonterminalsSet[i];
    if (i !== grammar.nonterminalsSet.length - 1) {
      stringGrammar += ",";
    }
  }
  stringGrammar += "|";
  for (let i = 0; i < grammar.terminalsSet.length; i += 1) {
    stringGrammar += grammar.terminalsSet[i];
    if (i !== grammar.terminalsSet.length - 1) {
      stringGrammar += ",";
    }
  }
  stringGrammar += "|" + grammar.start + "|";
  for (let i = 0; i < grammar.rulesSet.length; i += 1) {
    stringGrammar += grammar.rulesSet[i][0] + ",";
    if (grammar.rulesSet[i][1] === epsilon) {
      stringGrammar += substitutedEpsilon;
    } else {
      stringGrammar += grammar.rulesSet[i][1];
    }
    if (i !== grammar.rulesSet.length - 1) {
      stringGrammar += ",";
    }
  }
  return stringGrammar;
};
