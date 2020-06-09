    $(document).ready(function () {
        setInterval(function () {
            $.getJSON("inc/getData.php", function (data, status) {

                $(".js_waterTemp").html(data['waterTemp']);
                $(".js_luchtTemp").html(data['luchtTemp']);

                if (data['schaal'] == 0){
                    $(".js_schaal").html("C&#176;");
                    console.log("celcius");
                } else{
                    $(".js_schaal").html("F&#176;");
                    console.log("fahrenheit");
                }
            });
        }, 3000);
    });