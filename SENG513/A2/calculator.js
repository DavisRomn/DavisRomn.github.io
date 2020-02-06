$(document).ready(function(){

    var globalResult = 0;
    var justEquated = false;

    $(".operator").click( function() {
        let currentText = $("#result").text();
        
        if (currentText.length > 20) {
            return;
        }
        let operator = $(this)[0].value;

        if (currentText !== "0" && globalResult !== "ERROR") {
            if (justEquated) {
                $("#result").text(globalResult + operator);
                justEquated = false;
            } else {
                $("#result").text(currentText + operator);
            }
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
        $("#result-statement").text("");
        globalResult = 0;
        justEquated = false;
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
        justEquated = true;
        try {
            currentText = currentText.split('x').join('*');
            var result = eval(currentText);
    
            var decimalPlaces = result.toString().split('.');
            if (decimalPlaces.length > 1) {
                if (decimalPlaces[1].length > 6) {
                    result = result.toFixed(6);
                }
            }
    
            if (result === Infinity) {
                globalResult = "ERROR";
            }
    
            globalResult = result;
            $("#result-statement").text(currentText + " = " + result);
            $("#result").text(result);
        } catch(err) {
            globalResult = "ERROR"
            $("#result-statement").text(currentText + " = " + "ERROR");
        }
    });
});
