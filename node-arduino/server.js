var http = require('http');
var url = require("url");
var static = require('node-static');
var file = new(static.Server)('./public');

http.createServer(function (request, response) {
  console.log(url.parse(request.url));
  //response.writeHead(200, {'Content-Type': 'text/plain'});
  //response.end('dw9=lo\n');

  request.addListener('end', function () {
        file.serve(request, response);
    });
 
}).listen(8080);
console.log('Server running at http://127.0.0.1:8080/');
