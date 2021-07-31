var sidebar_hide = false;

$(document).ready(function() {
    //$("#content").width($(body).width() - $(".sidebar").width());
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
        }
        //$("#content").width($(body).width() - $(".sidebar").width());
        sidebar_hide = !sidebar_hide;
    });
});