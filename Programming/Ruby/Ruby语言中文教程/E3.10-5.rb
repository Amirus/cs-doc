#  E3.10-5.rb   ��ʾbreak, next, redo, retry

puts "��ʾbreak"
c='a'
  for i in 1..4
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      break
    end
    print i,c," "
  end
 puts "\n\n"
 
puts "��ʾnext" 
c='a'
  for i in 1..4    
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      next
    end
    print i,c," "
  end
  puts "\n\n"
  
puts "��ʾredo" 
c='a'
  for i in 1..4    
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      redo
    end
    print i,c," "
  end
  puts "\n\n"
  
  
puts "��ʾretry"
c='a'
  for i in 1..4    
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      retry
    end
    print i,c," "
  end
 puts "\n\n"
