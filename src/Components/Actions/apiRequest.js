import { backendConfig } from "../../config";
import { get, merge } from "lodash";

const solveErrors = response => {
  const contentType = response.headers.get("content-type");
  const result =
    contentType && contentType.includes("json")
      ? response.json()
      : response.text();

  return result.then((parsed: any) => {
    if (response.status >= 200 && response.status < 300) {
      return Promise.resolve(parsed);
    }

    const error = {
      status: response.status,
      errorCode: get(parsed, "error_code"),
      detail: get(parsed, "detail")
    };

    return Promise.reject(error);
  });
};

const apiRequest = (apiPath, options) => {
  const mainOptions = {
    headers: {
      Accept: "application/json"
    },
    mode: "cors"
  };

  const finalOptions = merge(mainOptions, options);

  if (finalOptions.body) {
    finalOptions.body = JSON.stringify(finalOptions.body);
    finalOptions.headers["Content-Type"] = "application/json";
  }

  const requestUrl = `http://${backendConfig.host}:${
    backendConfig.port
  }/api/${apiPath}`;
  console.log(finalOptions);
  return fetch(requestUrl, finalOptions).then(response =>
    solveErrors(response)
  );
};

export default apiRequest;
