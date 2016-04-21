 $(document).ready(function(e) {   
    // your code here
	var mouseX;
	var mouseY;
	
	$('.clickable').click(function(e) {
		if($(this).children().css('display') == 'none' && e.target == this) {
		mouseX = e.pageX;
		mouseY = e.pageY;
		var position = $(this).offset();
		console.log(e.clientX);
		console.log(e.pageX);
		console.log('showing');
		$(this).children().css({'top': mouseY-position.top, 'left':mouseX-position.left, 'position':'absolute'}).show();
		}}); 
		
		
	$('.exit').click(function() {
		$(this).parent().parent().hide();
		console.log('hidden');
	});
 });
