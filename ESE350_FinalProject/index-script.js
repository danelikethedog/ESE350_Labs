 $(document).ready(function(e) {   
    // your code here
	$('.hid').hide();
	$('.clickable').click(function() {
	$(this).children('.hid').toggle();
	});
 });
