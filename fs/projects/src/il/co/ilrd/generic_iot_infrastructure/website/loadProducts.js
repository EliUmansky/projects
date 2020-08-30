
function loadProducts(){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://localhost:8080/TomcatServer/Products", true);
    xhr.setRequestHeader("token", sessionStorage.getItem("token"));
    xhr.onload = function(){
        if (this.status != 200) {
            alert("Something is wrong, status = " + this.status);
            return;
        }
        var list = JSON.parse(xhr.response);
       
        var table = '<table>' + 
                        "<tr>" + 
                            "<th>Product ID</th>" + 
                            "<th>Product Name</th>" + 
                        "</tr>";
                            
        for (var i in list.products) {
                var product = list.products[i];
                table += 
                "<tr>" + 
                    "<td>" + product.productId + "</td>" + 
                    "<td>" + product.productName + "</td>" + 
                "</tr>";
        }

        table += "</table>";
        document.getElementById("products").innerHTML = table;
    }

    xhr.send();
}
