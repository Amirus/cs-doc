#E6.5-4.rb  

class Person    
  private    #����ķ����趨Ϊprivate
  def talk
    puts " already talk "
  end     
end

p1=Person.new
#p1.talk      private�������ܷ���

class Person      
  public :talk
end

p1.talk
