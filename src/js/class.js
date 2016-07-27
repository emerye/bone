// Constructor
function Foo(bar) {
  // always initialize all instance properties
  this.bar = bar;
  this.baz = 'baz'; // default value
  this.number = 32; 
}
// class methods
Foo.prototype.fooBar = function() {
  console.log('Prop  ' , this.bar,  "   " , "asdasdfasdfasdf",  this.number);

};

// export the class
module.exports = Foo;

var object = new Foo('Hello'); 
var obj1 = new Foo('There'); 
object.fooBar();
object.fooBar(); 
obj1.fooBar(); 


