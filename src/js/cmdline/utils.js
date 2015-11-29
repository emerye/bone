#!/usr/bin/env node


exports.sleep =  function sleep(time, callback) {
  var stop = new Date().getTime(); 
  while(new Date().getTime() < stop + time) { 
    ;
  }
  callback(); 
}



