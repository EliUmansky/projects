getCompName();

function addProduct(e){
    e.preventDefault();
    var nameVal = document.getElementById("productName").value;
    var compName = sessionStorage.getItem("companyName");
    var body = { 
        companyName: compName,
        productName: nameVal
    };

    var json = JSON.stringify(body);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "http://localhost:8080/TomcatServer/Products", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status == 401) {
            alert("Something is wrong, status = " + this.status + "\nPlease log in");
            location.replace("login.html");
            return;
        } else if (this.status != 200) {
            alert("Wrong input, try again\n Status = " + this.status);
            return;
        }
        location.replace("products.html");
    }

    xhr.send(json);
}

function editProduct(e){
    e.preventDefault();
    var prodId = sessionStorage.getItem('productId');
    var productName = document.getElementById("productName").value;
    var body = { 
                    productId: prodId,
                    productName: productName
                };

    var json = JSON.stringify(body);
    console.log(json);
    var xhr = new XMLHttpRequest();
    xhr.open("PUT", "http://localhost:8080/TomcatServer/Products", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status == 401) {
            alert("Something is wrong, status = " + this.status + "\nPlease log in");
            location.replace("login.html");
            return;
        } else if (this.status == 200) {
            location.replace("products.html");
        } else {
            console.log("status = " + this.status);
            location.replace("products.html");
        }
    }
    xhr.send(json);
}

function deleteProduct(prodId){
    console.log("edit product");
    var body = { 
        productId: prodId
    };

    var json = JSON.stringify(body);
    var xhr = new XMLHttpRequest();
    xhr.open("DELETE", "http://localhost:8080/TomcatServer/Products", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status == 401) {
            alert("Something is wrong, status = " + this.status + "\nPlease log in");
            location.replace("login.html");
            return;
        } else if (this.status != 200) {
            alert("Something is wrong, status = " + this.status);
            return;
        }
        location.replace("products.html");
    }

    xhr.send(json);
}

function loadProducts(){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://localhost:8080/TomcatServer/Products", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Something is wrong, status = " + this.status + "\nPlease log in");
            location.replace("login.html");
            return;
        }
        var list = JSON.parse(xhr.response);

        if (isEmpty(list.products)) {
            var empty = "<h1>You have no products yet</h1>";
            document.getElementById("products").innerHTML = empty;
            return;
        }

        var table = '<table>' + 
                        "<tr>" + 
                            "<th>Product ID</th>" + 
                            "<th>Product Name</th>" + 
                            "<th>Edit</th>" + 
                            "<th>Delete</th>" + 
                        "</tr>";
                            
        for (var i in list.products) {
                var product = list.products[i];
                table += 
                "<tr>" + 
                    "<td>" + product.productId + "</td>" + 
                    "<td>" + product.productName + "</td>" + 
                    "<td><button class='btn edit btn-primary' id='edit' onclick='editProd(" + product.productId + ")'/></td>" + 
                    "<td><button class='btn cancel btn-danger' id='remove' onclick='deleteProduct(" + product.productId + ")'/></td>" + 
                "</tr>";
        }

        table += "</table>";
        
        document.getElementById("products").innerHTML = table;
    }

    xhr.send();
}

function editProd(id){	
    sessionStorage.setItem("productId", id);
    location.replace("editProducts.html");
}

function isEmpty(obj) {
    for(var prop in obj) {
        if(obj.hasOwnProperty(prop))
            return false;
    }

    return true;
}

function getCompName() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://localhost:8080/TomcatServer/Companies", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Something is wrong, status = " + this.status + "\nPlease try again");
            location.replace("login.html");
            return;
        } else {
            let companyObj = JSON.parse(this.responseText);
            sessionStorage.setItem("companyName", companyObj.companyName);
            console.log(companyObj.companyName);
        }
    }

    xhr.send();
}