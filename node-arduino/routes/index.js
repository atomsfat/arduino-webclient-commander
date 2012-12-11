
/*
 * GET home page.
 */
var faye = require('faye');
var client = new faye.Client('http://localhost:8181/faye');
var url = require("url");
var cmd = "";
var count = 0;
exports.index = function(req, res){
  res.render('index', { title: 'Express' })
};

exports.arduino = function(req, res){
    count++;

    console.log(count + "  " +(new Date()) + "------------------");
    console.log( url.parse(req.url));

    client.publish('/data', url.parse(req.url) );
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end("{" + cmd + "}" );
};

exports.postData = function(req, res){

        cmd = req.param("cmd");
        console.log("cmd " + cmd);

        res.writeHead(200, {'Content-Type': 'text/json'});
        res.end('{status:ok}');


};




