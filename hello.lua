
-- print("put the video's URL?");

local a = 1;
while a <= 100 do
    if (a % 15 == 0) then
        print("FizzBuzz");
    elseif (a % 3 == 0) then
        print("Fizz");
    elseif (a % 5 == 0) then
        print("Buzz");
    else
        print(a);
    end
    a = a + 1;
    end