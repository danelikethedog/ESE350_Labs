 $(document).ready(function(e) {   
	var mouseX;
	var mouseY;
	
	$('.clickable').click(function(e) {
	
		var position = $(this).offset();
		mouseX = e.pageX - position.left;
		mouseY = e.pageY - position.top;
		
		if($(this).children(".dropdown").css('display') == 'none' && e.target == this) {
			$(this).children(".dropdown").css({'top': mouseY, 'left':mouseX, 'position':'absolute'}).show();
		} 
	}); 
	
	$('.picture').click(function(e) {
		
		var position = $(this).offset();
		mouseX = e.pageX - position.left;
		mouseY = e.pageY - position.top;
		
		if($(this).siblings(".dropdown").css('display') == 'none' && e.target == this) {
			$(this).siblings(".dropdown").css({'top': mouseY, 'left':mouseX, 'position':'absolute'}).show();
		} 
		
	});
	
	$('.inside-pic').click(function(e) {
		
		var position = $(this).offset();
		mouseX = e.pageX - position.left;
		mouseY = e.pageY - position.top;
		
		if($(this).parent().siblings(".dropdown").css('display') == 'none' && e.target == this) {
			$(this).parent().siblings(".dropdown").css({'top': mouseY, 'left':mouseX, 'position':'absolute'}).show();
		} 
		
	}); 	
		
	$('.exit').click(function() {
		$(this).parent().parent().hide();
		console.log('hidden');
	});
	
	
	$('.modify').click(function() {
		$(this).parent().siblings(".parameters").hide();
		$(this).parent().siblings(".form").show();
		$(this).hide();
	});
 });
