export * from './api/room_environment_monitor';
export * from './api/actions_smart_home';

import * as glob from "glob";
const camelCase = require("camel-case");

// The following block of code was grabbed from this tutorial https://codeburst.io/organizing-your-firebase-cloud-functions-67dc17b3b0da
const files = glob.sync('./**/*.f.js', { cwd: __dirname, ignore: './node_modules/**'});
for(let f=0,fl=files.length; f<fl; f++){
  const file = files[f];
  const functionName = camelCase(file.slice(0, -5).split('/').join('_')); // Strip off '.f.js'
  if (!process.env.FUNCTION_NAME || process.env.FUNCTION_NAME === functionName) {
    exports[functionName] = require(file);
  }
}