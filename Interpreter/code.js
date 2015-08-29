var v1 = 0;
var v2 = 0;
var v3 = 0;
var f = function (arg1) {
    v3 = 1;
    var v2 = 0;
    return function () {
        v2 = 1;
        v1 = 1;
        return arg1;
    }
}
("1")
/* sdfgsd fg sd fgsdf */
if (false);
else if (true);
else;
// test
var f2 = f();

var t = 5;
t /= 2;

var i = 10;
var j = 0;
while (i--)
    j += 2;

for (var i = 1000; i; i--) {
    j++;
}

for (;;) {
    if(!j--)break;
}

var bool = !10
var bool1 = !0
var bool3 = !true
var bool4 = !false;
var bool5 = true
var bool6 = false;

var k = 0;
var k2 = !-k;
var k3 = 4 / 3;


var x = "1.234";
x--;
x++;
x++