$(document).ready(function(){ 

    $(".operator").click( function() {
        let currentText = $("#result").text();
        
        if (currentText.length > 20) {
            return;
        }
        let operator = $(this)[0].value;

        if (currentText !== "0" && currentText !== "ERROR") {
            $("#result").text(currentText + operator);
        } else if (operator === '(') {
            $("#result").text(operator);
        }
    });

    $(".digit").click(function() {
        let currentText = $("#result").text();
        if (currentText.length > 20) {
            return;
        }
        let digit = $(this)[0].value;

        if (currentText === "0" || currentText === "ERROR") {
            $("#result").text(digit);
        }
        else {
            $("#result").text(currentText + digit);
        }
    });

    $(".backspace").click(function() {
        let currentText = $("#result").text();

        if (currentText !== '0' && currentText !== "ERROR") {
            if (currentText.length === 1) {
                currentText = '0';
            } else {
                currentText = currentText.slice(0, -1);
            }
            $("#result").text(currentText);
        } else if (currentText === "ERROR") {
            $("#result").text('0');
        }
    });

    $(".clear").click(function() {
        $("#result").text('0');
    });

    $(".decimal").click(function() {
        let currentText = $("#result").text();
        if (currentText.length > 20) {
            return;
        }
        if (!currentText.includes('.')) {
            // Append the decimal point
            $("#result").text(currentText + '.');
        }
    })

    $(".equals").click(function() {
        let currentText = $("#result").text();
        try {
            currentText = currentText.split('x').join('*');
            var result = eval(currentText);
    
            var decimalPlaces = result.toString().split('.');
            if (decimalPlaces.length > 1) {
                if (decimalPlaces[1].length > 5) {
                    result = result.toFixed(5);
                }
            }
    
            if (result === Infinity) {
                result = "ERROR";
            }
    
            $("#result").text(result);
        } catch(err) {
            console.log(err);
            $("#result").text("ERROR");
        }
    });
});
