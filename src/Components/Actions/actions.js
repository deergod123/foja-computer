import apiRequest from "./apiRequest";
import { stringToGrammar } from "../Grammars/components/Utils/GrammarParser";
import { stringToPrecedentialRelation } from "../Grammars/components/Utils/PrecedentialRelationParser";
import { strintgToSyntaxAnalysis } from "../Grammars/components/Utils/SyntaxAnalysisParser";

export const setGrammar1 = grammar1 => ({
  type: "Set grammar1",
  payload: grammar1,
  reducer: (state, grammar1Payload) => {
    return { ...state, grammar1: grammar1Payload };
  }
});

export const setGrammar2 = grammar2 => ({
  type: "Set grammar2",
  payload: grammar2,
  reducer: (state, grammar2Payload) => {
    return { ...state, grammar2: grammar2Payload };
  }
});

export const setGrammarForAnalysis = grammarForAnalysis => ({
  type: "Set grammar for Analysis",
  payload: grammarForAnalysis,
  reducer: (state, grammarForAnalysisPayload) => {
    return { ...state, grammarForAnalysis: grammarForAnalysisPayload };
  }
});

export const setPrecedentialRelation = precedentialRelation => ({
  type: "Set precedential relation",
  payload: precedentialRelation,
  reducer: (state, precedentialRelationPayload) => {
    return { ...state, precedentialRelation: precedentialRelationPayload };
  }
});

export const setAnalysisOfWord = analysisOfWord => ({
  type: "Set analysis of word",
  payload: analysisOfWord,
  reducer: (state, analysisOfWordPayload) => {
    return { ...state, analysisOfWord: analysisOfWordPayload };
  }
});

export const setWordForAnalysis = wordForAnalysis => ({
  type: "Set word for Analysis",
  payload: wordForAnalysis,
  reducer: (state, wordForAnalysisPayload) => {
    return { ...state, wordForAnalysis: wordForAnalysisPayload };
  }
});
export const setEquivalencyAnswer = equivalencyAnswer => ({
  type: "Set answer for equivalency",
  payload: equivalencyAnswer,
  reducer: (state, equivalencyAnswerPayload) => {
    return { ...state, equivalencyAnswer: equivalencyAnswerPayload };
  }
});

export const toReducedNormalForm = (grammar, setGrammar) => dispatch => {
  const data = {
    grammar: grammar
  };

  return apiRequest(`grammarRequest/reducedNormalForm`, {
    method: "POST",
    body: data
  })
    .then(result => {
      dispatch(setGrammar(stringToGrammar(result)));
    })
    .catch(e => {
      throw e;
    });
};

export const toEpsilonFreeForm = (grammar, setGrammar) => dispatch => {
  const data = {
    grammar: grammar
  };

  return apiRequest(`grammarRequest/epsilonFreeForm`, {
    method: "POST",
    body: data
  })
    .then(result => {
      dispatch(setGrammar(stringToGrammar(result)));
    })
    .catch(e => {
      throw e;
    });
};

export const isEquivalent = (grammar1, grammar2) => dispatch => {
  const data = {
    grammar1: grammar1,
    grammar2: grammar2
  };

  return apiRequest(`grammarRequest/equivalence`, {
    method: "POST",
    body: data
  })
    .then(result => {
      const grammar = result.split("|");
      dispatch(
        setEquivalencyAnswer({
          equivalency: grammar[1] === "0" ? false : true,
          answer: grammar[0]
        })
      );
    })
    .catch(e => {
      throw e;
    });
};

export const isPrecedential = stringGrammar => dispatch => {
  const data = {
    grammar: stringGrammar
  };

  return apiRequest(`grammarRequest/isPrecedential`, {
    method: "POST",
    body: data
  })
    .then(result => {
      const splitedResult = result.split("#");
      dispatch(
        setGrammarForAnalysis({
          grammar: stringToGrammar(stringGrammar),
          isPrecedential: splitedResult[0] === "0" ? false : true
        })
      );
      dispatch(
        setPrecedentialRelation(
          splitedResult[0] === "0"
            ? null
            : stringToPrecedentialRelation(splitedResult[1])
        )
      );
    })
    .catch(e => {
      throw e;
    });
};

export const getWordSyntaxAnalysis = (grammar, word) => dispatch => {
  const data = {
    grammar: grammar,
    word: word
  };

  return apiRequest(`grammarRequest/getWordSyntaxAnalysis`, {
    method: "POST",
    body: data
  })
    .then(stringCommands => {
      const commands = strintgToSyntaxAnalysis(stringCommands);
      dispatch(setAnalysisOfWord(commands));
      dispatch(setWordForAnalysis(word));
    })
    .catch(e => {
      throw e;
    });
};
