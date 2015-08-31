
print("PRINTED FROM JS");

var all_tests_passed = function () {

    var assert_equals = function (fn, val) {
        var test = fn();
        var ret = "";
        if (test === val) {
            ret += "Passed : ";
        } else {
            ret += "Failed : ";
            atp = false;
        }
        return ret + val + " === " + test
    }
    //asd as
    /*
    as asd asd ** /dfasdfasdf
    *///asdas
    //*asxdfasdf
    /*/**
    ***/

    var closure_test = function () {
        var a = 0;
        var b = 2;
        var c = 0;
        var d = 4;
        var e = 5;
        var f = function (g) {
            a = 1;
            var b = -1;
            b = 0;
            var d = -1;
            h = 7;
            var l = f;
            return function (g) {
                var e = -1;
                e = 0;
                c = 3;
                d = 0;
                i = 8;
                var z = l;
                try {
                    j = 0;
                } catch (e) {
                    k = 10;
                }
                return g;
            }
        }
        var h = 0;
        var i = 0;
        var k = 0;
        var x = f(0)
        var g = x(6);
        var j = 9;

        return "" + a + b + c + d + e + g + h + i + j + k;
        // comment
    }

    var for_test = function () {
        var j = 0;
        var i = 10;
        var k = 0;
        var l = 20;

        var b = false;
        for (var i = 0; i < 20; i++) {
            var l = 0;
            b = !b;
            if (b)++j;
            l = 10;
        }
        for (var m = 0; m < 18; m++) {
            k++;
            if (m == 9) break;
        }

        var n = 0;
        var o = 0;
        for (; ; n++) {
            if (n >= 10) {
                o = 10;
                break;
            }
            o = 19;
        }

        var p = -12;
        for (; p < 10 ;) {
            p += 2
        }
        for (var i = -10; ;) {
            if (++i == 10) break;
            continue;
            j = 1;
            k = 1;
        }
        return "" + j + l + k + i + n + o + p;
    }


    var while_test = function () {
        var i = 0
        while (i < 10) i++;

        var j = 20;
        var f = 0;
        var x = 0;
        while (true) {
            var sdfgas = 0;
            f = function (e) {
                x = e;
            }
            if (--j == 10) break;
        }
        f(10);
        var k = 0;
        var l = 0;
        try {
            while (true) //sadfg asdf
                if (++k == 10)
                    /* asd*/throw 10
        }
        catch (e) {
            l = e;
        }

        return "" + i + j + k + l + x;

    };

    var try_catch = function () {
        var f = function (b, str) {
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
        } catch (e) {
            m2 = e;
        }

        var m3 = "";
        try {
            try {
                throw " inner"
            } catch (e) {
                throw e + "_outer";
            }
        } catch (e) {
            m3 = e;
        }

        return "" + a + b + " " + m + m2 + m3;

    }


    function named_function() {
        return 10;
    }

    function shift_test() {
        var is8 = 32 >> 2;
        var is16 = 1 << 4;
        var is32 = 128;
        is32 >>= 2;
        var is4 = 1;
        is4 <<= 2;

        return is4 === 4 && is8 === 8 && is16 === 16 && is32 === 32;
    }

    var atp = true;
    var shift = assert_equals(shift_test, true);
    var named_fn = assert_equals(named_function, 10);
    var closure = assert_equals(closure_test, "12345678910");
    var for_r = assert_equals(for_test, "10101010101010");
    var for_w = assert_equals(while_test, "1010101010");
    var try_catch_r = assert_equals(try_catch, "11 variable 'undef' is undefined my_err inner_outer");

    return atp;

}();