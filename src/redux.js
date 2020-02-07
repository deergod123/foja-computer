import { createStore, applyMiddleware } from "redux";
import { initialState } from "./Components/state";
import thunk from "redux-thunk";

const getInitialState = () => {
  return {
    ...initialState
  };
};

const rootReducer = (state, action) => {
  console.log("Action: ", action.type);
  console.log("Old state: ", state);
  if (action.reducer === undefined) {
    return state;
  } else {
    const newState = action.reducer(state, action.payload);
    console.log("New state: ", newState);
    return newState;
  }
};

export const configuredStore = () => {
  return createStore(rootReducer, getInitialState(), applyMiddleware(thunk));
};
