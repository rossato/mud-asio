place = World.Create(
   "Place", 1000,
   {
      description = [[
Greetings from beautiful Place! You are in a location
that has fantastic vistas and a long description that
goes on for three whole lines.]],
      exits = {
         e = 1001
      }
   }
)

book = World.Create(
   "book", 1002,
   {
      description = "What a fine book!",
      parent = place
   }
)

other_place = World.Create(
   "Other Place", 1001, 
   {
      description = "You found the other place!",
      exits = {
         w = 1000
      }
   }
)
