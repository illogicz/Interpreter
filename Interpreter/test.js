var assert_equals = function(fn, val)
{
    var test = fn();
    var ret = "";
    if (test === val) {
        ret += "Passed : ";
    } else {
        ret += "Failed : ";
    }
    return ret + val + " === " + test
}


var closure_test = function()
{
    var a = 0;
    var b = 1;
    var c = 0;
    var d = 1;
    var e = 1;
    var f = function(g)
    {
        a = 1;
        var b = -1;
        b = 0;
        var d = -1;
        h = 1;
        //j = 1;
        return function(g)
        {
            var e = -1;
            e = 0;
            c = 1;
            d = 0;
            i = 1;
            return g;
            j = 0;
        }
    }
    var h = 0;
    var i = 0;
    var g = f(0)(1);
    var j = 1;

    return "" + a + b + c + d + e + g + h + i + j;

}

var for_test = function()
{
    var j = 0;
    var i = 10;
    var k = 0;
    var l = 10;

    var b = false;
    for (var i = 0; i < 20; i++) {
        var l = 0;
        b = !b;
        if (b)++j;
        l = 20;
    }
    for (var m = 0; m < 18; m++) {
        k++;
        if (m == 9) break;
    }

    var n = 0;
    var o = 0;
    for (; ;n++) {
        if (n >= 10) {
            o = 10;
            break;
        }
        o = 19;
    }

    var p = -12;
    for (; p < 10 ; ) {
        p += 2
    }
    for (var i = 9;;) {
        if (++i == 18) break;
        continue;
        j = 1;
        k = 1;
    }
    return "" + j + l + k + i + n + o + p;
}


var try_catch = function()
{
    var f = function(b, str)
    {
        if (b) {
            throw str;
        }
    }


    var a = 0;
    var m = "";
    try {
        undef = 0;
    } catch (e) {
        m = e;
        a = 1;
    }

    var b = 0
    try {
        b = 1;
    } catch (e) {
        b = -1;
    }

    var m2 = "";
    try {
        f(true, " my_err");
    } catch(e) {
        m2 = e;
    }

    var m3 = "";
    try{
        try {
            throw " inner"
        } catch (e) {
            throw e + "_outer";
        }
    } catch(e) {
        m3 = e;
    }

    return "" + a + b + " " + m + m2 + m3;

}


var closure = assert_equals(closure_test, "111111111");
var for_r = assert_equals(for_test, "10101010101010");
var try_catch_r = assert_equals(try_catch, "11 variable 'undef' is undefined my_err inner_outer");
