loginform.addEventListener("submit",loginToService);

function loginToService(e){
    e.preventDefault();
    var emailVal = document.getElementById("email").value;
    var pwVal = document.getElementById("password").value;
    var body = { 
                    email: emailVal,
                    password: pwVal
                };

    var json = JSON.stringify(body);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "http://localhost:8080/TomcatServer/Login", true);
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Incorrect email or password");
	    return;
        } 

	var token = xhr.response;
        sessionStorage.setItem("token", token);
        location.replace("products.html");
    }

    xhr.send(json);
}
