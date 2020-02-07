const express = require("express");
const port = 8080;
const cors = require("cors");
const api = require("./api");
const bodyParser = require("body-parser");
const app = express();

app.use(cors());
app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header(
    "Access-Control-Allow-Headers",
    "Origin, X-Requested-With, Content-Type, Accept"
  );
  next();
});

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

app.use("/api", api.router);

// console.log that your server is up and running
app.listen(port, () => console.log(`Listening on port ${port}`));
