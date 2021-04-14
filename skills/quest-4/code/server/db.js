// Create or open the underlying LevelDB store
var level = require('level')
var express = require('express');
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');
var url = require('url');
var db = level('./mydb', {valueEncoding: 'json'});
var HTMLport = 3333;
// Random number function -- this is a helper function to generate dummy data
function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min + 1) ) + min;
}

var id = [];
var vote = [];
var j = 0;
 fs.readFile('data.csv','utf8',function(err,data){
  lines = data.split("\n");
  //console.log(lines[0]);
  for(var i = 1; i < lines.length; i++){
    tabs = lines[i].split(',');

    //console.log(tabs[0]);

    id[j] = tabs[0];
    vote[j] = tabs[1];
    j++;
    console.log(id[j])
    console.log(vote[j]);;
  }
  console.log(id);
  console.log("line printed");
});


app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
  console.log('a user connected');
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });
});
// Function to stream from database
function readDB(arg) {
  db.createReadStream()
    .on('data', function (data) {
      //console.log(data.key, '=', data.value)
      // Parsed the data into a structure but don't have to ...
      var dataIn = {[data.key]: data.value};
      // Stream data to client
      io.emit('message', dataIn);
    })
    .on('error', function (err) {
      console.log('Oh my!', err)
    })
    .on('close', function () {
      console.log('Stream closed')
    })
    .on('end', function () {
      console.log('Stream ended')
    })
}

// When a new client connects
var clientConnected = 0; // this is just to ensure no new data is recorded during streaming
io.on('connection', function(socket){
  console.log('a user connected');
  clientConnected = 0;

  // Call function to stream database data
  readDB();
  clientConnected = 1;
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });
});

// Listening on localhost:3000
// http.listen(3000, function() {
//   console.log('listening on *:3000');
//
// });

var i = 0;

function intervalFunc() {
  if (clientConnected == 1) {

    // Get current time
    // var date = time[i];
    // //var tempPoints = temp[1];
    // // Fill in data structure
    // var value = [{id:id[i], temp: temp[i], smoke: smoke[i]}];
    var ID = id[i];
    var VOTE = vote[i];
    i++;



    // Put structure into database based on key == date, and value
    db.put(ID, VOTE, function (err) {
      if (err) return console.log('Ooops!', err) // some kind of I/O error
    })
    console.log("data saved");
    // Parse data to send to client
    var msg = {[ID]: VOTE};
    // Send to client
    //if(q.time == date){ //check if if the key is same as query string
      io.emit('message', msg);
      console.log("message sent");
    //}


    // Log to console
    console.log(Object.keys(msg));
  }
}
// Do every 1500 ms
setInterval(intervalFunc, 1500);

http.listen(HTMLport, () => {
 console.log(`App listening at http://localhost:${HTMLport}`)
})
