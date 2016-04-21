 $(document).ready(function(e) {   
    // your code here
	var mouseX;
	var mouseY;
	$('.clickable').click(function(e) {
		if($('.dropdown').css('display') == 'none' && e.target == this) {
		mouseX = e.pageX;
		mouseY = e.pageY;
		console.log('showing');
		$('.dropdown').css({'top': mouseY-70, 'left':mouseX-465, 'position':'absolute'}).show();
		}}); 
		
		
	$('.exit').click(function() {
		$('.dropdown').hide();
		console.log('hidden');
	});
 });
