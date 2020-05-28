$( document ).ready(function() {
    $( ".inloggen" ).click(function() {
        var userName = $("#userName")[0].value;
        var password = $("#wachtwoord")[0].value;

        $.ajax({
            url : "inc/login.php",
            type: "POST",
            data : {username:userName,pass:password},
            success: function(data, textStatus, jqXHR)
            {
                console.log(data)
            }
        });
    });

    $( ".js-addKratje" ).click(function() {
        var addOrRemove = true;
        $.ajax({
            url : "inc/addOrRemove.php",
            type: "POST",
            data : {addOrRemove:addOrRemove},
            success: function(data, textStatus, jqXHR)
            {
                location.reload();
                console.log(data)
            }
        });
    });

    $( ".js-removeKratje" ).click(function() {
        var addOrRemove = false;
        $.ajax({
            url : "inc/addOrRemove.php",
            type: "POST",
            data : {addOrRemove:addOrRemove},
            success: function(data, textStatus, jqXHR)
            {
                location.reload();
                console.log(data)
            }
        });
    });

    const items = document.querySelectorAll(".accordion a");

    function toggleAccordion(){
        this.classList.toggle('active');
        this.nextElementSibling.classList.toggle('active');
    }

    items.forEach(item => item.addEventListener('click', toggleAccordion));
});


