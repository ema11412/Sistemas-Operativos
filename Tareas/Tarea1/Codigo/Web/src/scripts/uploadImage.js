//Script uploadImage.js

//var of button to choose input file
const realFileBtn = document.getElementById("inpFile");
//var button 
const customBtn = document.getElementById("custom-button");
//Preview text of image
const customTxt = document.getElementById("custom-text");
//Canvas of preview image 
const previewContainer = document.getElementById("imagePreview");
//Preview image
const previewImage = previewContainer.querySelector(".image-preview__image");
//Default text "Preview image", change to name of image.PNG
const previewDefaultText = previewContainer.querySelector(".image-preview__default-text");

//Action click to call a function
customBtn.addEventListener("click", function(){
    realFileBtn.click();
});

//Function choose image
realFileBtn.addEventListener("change", function(){
    //Image file, take values of image
    const file = this.files[0];
    //Name of image file
    var imageName = file.name;
    //Var to compare if image is .PNG
                            //gif|jpg|png|jpeg
    var isGood = (/\.(?=gif|jpg|png|jpeg)/gi).test(imageName);

    //if file image is PNG
    if (isGood){
        //Show only name image, delete path
        customTxt.innerHTML = realFileBtn.value.match(/[\/\\]([\w\d\s\.\-\(\)]+)$/)[1];
        //read image information
        const reader = new FileReader();
        //show the text name of image and image preview
        previewDefaultText.style.display = "none";
        previewImage.style.display = "block";

        //Load image to show in preview canvas
        reader.addEventListener("load", function() {
        previewImage.setAttribute("src", this.result);
        });
        reader.readAsDataURL(file);

    //if file image is not PNG
    } else{
        //Default text and display derfault preview image
        customTxt.innerHTML = "No file chosen. yet";
        previewDefaultText.style.display = null;
        previewImage.style.display = null;
        previewImage.setAttribute("src", "");
    }
});

//Function send image to server
$(document).ready(function(){
    //if click send button 
    $('#send-button').click(function(){
        //Take information of image file
        var files = $('#inpFile')[0].files[0];
        //Take information of input text IP
        var ip = $('#ip').val();
        //Take information of input text PORT
        var port = $('#port').val();
        //Ajax function
        $.ajax({
            //url of server
            url: 'http://'+ip+':'+port,
            //Type of request POST
            type: 'post',
            //Data to send (image file .PNG)
            data: files,
            //Content Type text/plain, server reads this format
            contentType: 'text/plain',
            //No proccess Data
            processData: false,
            //Succes process
            success: function(response){
                if(response !=0){
                    alert('File uploaded');
                }else{
                    alert('File not uploaded');
                }
            }
        });
    });

    //Function send image to server
    $('#init-button').click(function(){
        //Take information of input text IP
        var ip = $('#ip').val();
        //Take information of input text PORT
        var port = $('#port').val();
        $.ajax({
            //url of server
            url: 'http://'+ip+':'+port,
            //Type of request POST
            type: 'post',
            //Send message
            data: 'init\0',
            //Content Type text/plain, server reads this format
            contentType: 'text/plain',
            //No proccess Data
            processData: false,
            //Succes process
            success: function(response){
                if(response !=0){
                    alert('Init');
                }else{
                    alert('Not Init');
                }
            }
        });
    });


});

