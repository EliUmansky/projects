signupform.addEventListener("submit",addCompany);

function addCompany(e){
    e.preventDefault();
    var nameVal = document.getElementById("companyName").value;
    var emailVal = document.getElementById("email").value;
    var pwVal = document.getElementById("password").value;
    var confPw = document.getElementById("confirmPassword").value;
    if (pwVal != confPw) {
	alert("Passwords don't match");
	return;
    }

    var addressElement = document.getElementById("address");
    var addressVal = null;
    if (null != addressElement) {
        addressVal = addressElement.value;
    } else { 
        addressVal = "";
    }

    var body = { 
                    companyName: nameVal,
                    email: emailVal,
                    password: pwVal,
                    address: addressVal
                };

    var json = JSON.stringify(body);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "http://localhost:8080/TomcatServer/Companies", true);
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Something is wrong, status = " + this.status);
        } else {
    	    alert("You're successfully joined us!\nPlease log in");
	    location.replace("login.html");
	}
    }

    xhr.send(json);
}

function loadCompanyDetails(){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://localhost:8080/TomcatServer/Companies", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Wrong input, status = " + this.status);
            return;
        }
        var companyDetails = JSON.parse(xhr.response);

        var table = '<table>' + 
                    "<tr>" + 
                        "<th>Company ID</th>" + 
                        "<th>Company Name</th>" + 
                        "<th>Email</th>" + 
                        "<th>Address</th>" + 
                    "</tr>";
    
        table += 
                "<tr>" + 
                    "<td>" + companyDetails.companyId + "</td>" + 
                    "<td>" + companyDetails.companyName + "</td>" + 
                    "<td>" + companyDetails.email + "</td>" + 
                    "<td>" + companyDetails.address + "</td>" + 
                    // "<td><button class='editbtn'>edit</button></td>" + 
                "</tr>";

        table += "</table>";
        document.getElementById("companyDetails").innerHTML = table;
    }

    xhr.send();
}

function editCompany(e){
    e.preventDefault();
    var nameVal = document.getElementById("companyName").value;
    var pwVal = document.getElementById("password").value;
    var addressElement = document.getElementById("address");
    var addressVal = null;
    if (null != addressElement) {
        addressVal = addressElement.value;
    }
    var body = { 
                    companyName: nameVal,
                    password: pwVal,
                    address: addressVal
                };

    var json = JSON.stringify(body);
    var xhr = new XMLHttpRequest();
    xhr.open("PUT", "http://localhost:8080/TomcatServer/Companies", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status == 401) {
            alert("Something is wrong, status = " + this.status + "\nPlease log in");
            location.replace("login.html");
            return;
        } else if (this.status != 200) {
            alert("Something went wrong, status = " + this.status);
            location.replace("login.html");
            return;
        }
       
        location.replace("compDetails.html");
    }

    xhr.send(json);
}

function deleteCompany(){
    var xhr = new XMLHttpRequest();
    xhr.open("DELETE", "http://localhost:8080/TomcatServer/Companies", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
	if (this.status == 401) {
            alert("Something is wrong, status = " + this.status + "\nPlease log in");
            location.replace("login.html");
            return;
        }else if (this.status != 200) {
            alert("Processing data failed, please try again");
            location.replace("compDetails.html");
            return;
        }
       
        location.replace("welcome.html");
    }

    xhr.send();
}

function loadCompanyName(){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://localhost:8080/TomcatServer/Companies", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Session ended, please log in");
            location.replace("login.html");
            return;
        }
        var companyDetails = JSON.parse(xhr.response);
        var company = companyDetails.companyName;
        document.getElementById("companyName").innerHTML = company;
    }

    xhr.send();
}