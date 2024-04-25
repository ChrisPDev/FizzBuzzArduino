using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ArduinoAPI.Data;
using ArduinoAPI.Models;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class GameController : ControllerBase
    {
        private readonly ApplicationDbContext _context;

        public GameController(ApplicationDbContext context)
        {
            _context = context;
        }

        // GET: api/GetGame
        [HttpGet("GetGame")]
        public async Task<ActionResult<IEnumerable<Game>>> GetGame()
        {
            return await _context.Games.ToListAsync();
        }

        // GET: api/GetGame/5
        [HttpGet("GetGame/{id}")]
        public async Task<ActionResult<Game>> GetGame(int id)
        {
            var game = await _context.Games.FindAsync(id);

            if (game == null)
            {
                return NotFound();
            }

            return game;
        }

        // PUT: api/PutGame/5
        [HttpPut("PutGame/{id}")]
        public async Task<IActionResult> PutGame(int id, Game game)
        {
            if (id != game.Id)
            {
                return BadRequest();
            }

            _context.Entry(game).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!GameExists(id))
                {
                    return NotFound();
                }
                else
                {
                    throw;
                }
            }

            return NoContent();
        }

        // POST: api/PostGame
        [HttpPost("PostGame")]
        public async Task<ActionResult<Game>> PostGame(Game game)
        {
            _context.Games.Add(game);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetGame", new { id = game.Id }, game);
        }

        // POST: api/PostGame/5
        [HttpPost("PostGame/{answerStreak}")]
        public async Task<ActionResult<Game>> PostGame(int answerStreak, Game game)
        {
            game.answerStreak = answerStreak; // Set the answer streak

            _context.Games.Add(game);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetGame", new { id = game.Id }, game);
        }


        // DELETE: api/DeleteGame/5
        [HttpDelete("DeleteGame/{id}")]
        public async Task<IActionResult> DeleteGame(int id)
        {
            var game = await _context.Games.FindAsync(id);
            if (game == null)
            {
                return NotFound();
            }

            _context.Games.Remove(game);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool GameExists(int id)
        {
            return _context.Games.Any(e => e.Id == id);
        }
    }
}
