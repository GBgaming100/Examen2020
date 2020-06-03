$( document ).ready(function() {
    $( ".js_C" ).click(function() {
        $( ".js_C" ).addClass( "d-none" );
        $( ".js_F" ).removeClass( "d-none" );

        var schaal = "F";
        $.ajax({
            url : "inc/switchSchaal.php",
            type: "POST",
            data : {schaal:schaal},
            success: function(data)
            {
                console.log(data);
            }
        });

    });

    $( ".js_F" ).click(function() {
        $( ".js_F" ).addClass( "d-none" );
        $( ".js_C" ).removeClass( "d-none" );

        var schaal = "C";
        $.ajax({
            url : "inc/switchSchaal.php",
            type: "POST",
            data : {schaal:schaal},
            success: function(data)
            {
                console.log(data);
            }
        });
    });
});