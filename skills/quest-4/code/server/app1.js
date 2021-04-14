// Modules
var level = require('level')
var express = require('express');
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');
var url = require('url');
const HTMLport = 3000;



var fs = require('fs');
fs.writeFile('data.csv', 'ID,VOTE\n', function(err){
 if(err) throw err;
 console.log('File created')
})


// Required modules
var dgram = require('dgram');

// Port and IP
var PORT = 3333;//3333;
var HOST = '10.0.0.115';
//check if 115 is Pi or Laptop

// Create socket
var server = dgram.createSocket('udp4');

// Create UDP server
server.on('listening', function () {
   var address = server.address();
   console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// On connection, print out received message
server.on('message', function (message, remote) {
   console.log(remote.address + ':' + remote.port +' - ' + message);
   //console.log(message);
   // Send Ok acknowledgement

     server.send("Ok!",remote.port,remote.address,function(error){
       if(error){
         console.log('MEH!');
       }
       else{
         console.log('Sent: Ok!');
       }
     });

   // Append data to CSV file
   fs.appendFile('data.csv', message + "\n", function(err){
     if(err) throw err;
     console.log('Data appended')
   })

});

// Bind server to port and IP
server.bind(PORT, HOST);





// app.listen(HTMLport, () => {
//  console.log(`App listening at http://localhost:${HTMLport}`)
// })

//at this point, the program will have already created 'data.csv'
//as well as append the data from the ESP Leader. It can be used to save data
//into DB
