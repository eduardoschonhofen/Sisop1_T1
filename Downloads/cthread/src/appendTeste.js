var http = require('http');
var url = require('url');
var fs = require('fs');

http.createServer(function(req,res)
{
res.writeHead(200,{'Content-Type':'text/html'});
var parser=url.parse(req.url,true).query;
var nome ="Nome:"+parser.nome+"\n";
var idade ="Idade:"+parser.idade+"\n";
var cargo ="Tarefa:"+parser.tarefa+"\n";
fs.appendFile('DadosAppend.txt',nome+idade+cargo, function(err)
{
  if (err) throw err;
  console.log("Salvei!");
});
end("Tudo salvo!");
}).listen(8080);
