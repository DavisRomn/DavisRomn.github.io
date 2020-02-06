$(document).ready(function(){

    // Global variable to store the most recent result.
    var globalResult = 0;
    // Global variable to state whether equals was just pressed
    var justEquated = false;


    // If an operator button is clicked [+, -, /, *, (, )]
    $(".operator").click( function() {
        // Get the current text that has been entered
        let currentText = $("#result").text();
        
        // If the text is over 20 chracters long, don't add more
        if (currentText.length > 20) {
            return;
        }
        // Grab the value of the operator that is to be added
        let operator = $(this)[0].value;

        // If the current text is not simply a zero nor an error
        if (currentText !== "0" && globalResult !== "ERROR") {
            // If the equals button was recently pressed
            if (justEquated) {
                // Utilize the global result
                $("#result").text(globalResult + operator);
                justEquated = false;
            } else {
                // Utilize the text currently entered
                $("#result").text(currentText + operator);
            }
        } else if (operator === '(') {
            // If the operator is a left bracket, and currentText is 0 or an error, replace the text with just a left bracket
            $("#result").text(operator);
        }
    });

    // If a number button is clicked [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    $(".digit").click(function() {
        // Set the currentText to the current text of the display
        let currentText = $("#result").text();
        // If the text is over 20 chracters long, don't add more
        if (currentText.length > 20) {
            return;
        }
        // Grab the value of the digit that is to be added
        let digit = $(this)[0].value;

        // If the current text is an error or only a zero, replace the display with just the digit
        if (currentText === "0" || currentText === "ERROR") {
            $("#result").text(digit);
        }
        else {
            // Else append it to the string
            $("#result").text(currentText + digit);
        }
    });

    // If the backspace, CE, is pressed
    $(".backspace").click(function() {
        justEquated = false;
        // Get the current text
        let currentText = $("#result").text();

        // If the currentText is not simply a zero nor an error
        if (currentText !== '0' && currentText !== "ERROR") {
            // If the currentText has only one digit, replace it with zero
            if (currentText.length === 1) {
                currentText = '0';
            } else {
                // Else cut off the last character
                currentText = currentText.slice(0, -1);
            }
            // Update the display
            $("#result").text(currentText);
        } else if (currentText === "ERROR") {
            // If the currentText was error, reset to 0
            $("#result").text('0');
        }
    });

    // If the clear button, C, is pressed
    $(".clear").click(function() {
        // Set the main display to 0
        $("#result").text('0');
        // Set the result statement display to blank
        $("#result-statement").text("");
        // Reset the global vars
        globalResult = 0;
        justEquated = false;
    });

    // If the decimal button is pressed
    $(".decimal").click(function() {
        // Get the current text
        let currentText = $("#result").text();
        // If the text is over 20 chracters long, don't add more
        if (currentText.length > 20) {
            return;
        }
        // If the currentText's last character is not also a decimal
        if (currentText.charAt(currentText.length - 1) !== '.') {
            // Append the decimal point
            $("#result").text(currentText + '.');
        }
    })

    // If the equals button, =, is pressed
    $(".equals").click(function() {
        // Get the current text
        let currentText = $("#result").text();
        // Update the gloabl var to say that the equals button has been pressed
        justEquated = true;
        // Attempt to perform calculations
        try {
            // Replace all x's with * to properly facilitate multiplication
            currentText = currentText.split('x').join('*');
            // Evaluate the string to get the result
            var result = eval(currentText);
    
            // Split the string on the decimal point, if it has one, to count the number of decimal points
            var decimalPlaces = result.toString().split('.');
            // If the result has decimal places
            if (decimalPlaces.length > 1) {
                // If the result has more than eight decimal places
                if (decimalPlaces[1].length > 8) {
                    // Force the result to only have eight decimal places
                    result = result.toFixed(8);
                }
            }

            // If the result is Infinity (divide by 0 occured)
            if (result === Infinity) {
                // Set the globalResult to ERROR
                globalResult = "ERROR";
            }

            // If the result has more than 14 characters, 
            if (result.toString().length >= 14) {
                // Convert it to an Exponential
                result = result.toExponential(6);
            }

            globalResult = result;
            $("#result-statement").text(currentText + " = " + result);
            $("#result").text(result);
        } catch(err) { // Catch an error if it occurs
            // Set the globalResult to ERROR
            globalResult = "ERROR"
            // Update the result statement to show the ERROR
            $("#result-statement").text(currentText + " = " + "ERROR");
        }
    });
});
