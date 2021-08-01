var sidebar_hide = false;

$(document).ready(function() {
    //$("#content").width($(body).width() - $(".sidebar").width());

    var width = $(window).width();

    if (width < 800 && !sidebar_hide) {
        $(".sidebar").animate({
            width: "25px"
        }, 500);
        $(".sidebar_text").hide();
        sidebar_hide = true;
    } else if (width >= 800 && !sidebar_hide) {
        $(".sidebar").animate({
            width: "240px"
        }, 500);

        $(".sidebar_text").show();
        sidebar_hide = false;
    }

    $(window).resize(function() {
        var width = $(window).width();

        if (width < 800 && !sidebar_hide) {
            $(".sidebar").animate({
                width: "25px"
            }, 500);
            $(".sidebar_text").hide();
            sidebar_hide = true;
        } else if (width >= 800 && !sidebar_hide) {
            $(".sidebar").animate({
                width: "240px"
            }, 500);

            $(".sidebar_text").show();
            //$("#map").width($(window).width());
            //window.scrollLeft = 0;
            sidebar_hide = false;
        }
    });



    $("#btn_collapse").click(function() {
        if (sidebar_hide) {
            //Se la sidebar è nascosta la mostro
            $(".sidebar").animate({
                width: "240px"
            }, 500);

            $(".sidebar_text").show();
        } else {
            //Se la sidebar è visibile la nascondo
            $(".sidebar").animate({
                width: "25px"
            }, 500);
            $(".sidebar_text").hide();
            //$("#map").width($(window).width());
            //window.scrollLeft = 0;
        }
        //$("#content").width($(body).width() - $(".sidebar").width());
        sidebar_hide = !sidebar_hide;
    });


});