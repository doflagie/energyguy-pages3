export async function onRequest(context) {
    const { request, env } = context;

    const corsHeaders = {
        'Content-Type': 'application/json',
        'Access-Control-Allow-Origin': '*',
        'Access-Control-Allow-Methods': 'GET, POST, OPTIONS',
        'Access-Control-Allow-Headers': 'Content-Type',
    };

    if (request.method === 'OPTIONS') {
        return new Response(null, { status: 204, headers: corsHeaders });
    }

    // Month is always set server-side — clients cannot forge it
    const now   = new Date();
    const month = `${now.getUTCFullYear()}-${String(now.getUTCMonth() + 1).padStart(2, '0')}`;
    const kvKey = `scores:${month}`;

    if (request.method === 'GET') {
        const raw    = await env.GAUNTLET_SCORES.get(kvKey);
        const scores = raw ? JSON.parse(raw) : [];
        return new Response(JSON.stringify({ month, scores }), { headers: corsHeaders });
    }

    if (request.method === 'POST') {
        let body;
        try { body = await request.json(); }
        catch { return new Response(JSON.stringify({ error: 'Bad JSON' }), { status: 400, headers: corsHeaders }); }

        const { name, correct, seconds } = body;

        // Validate
        if (typeof name !== 'string' || name.trim().length < 1 || name.trim().length > 20) {
            return new Response(JSON.stringify({ error: 'Name must be 1–20 characters' }), { status: 400, headers: corsHeaders });
        }
        if (!Number.isInteger(correct) || correct < 0 || correct > 1500) {
            return new Response(JSON.stringify({ error: 'Invalid correct count' }), { status: 400, headers: corsHeaders });
        }
        if (!Number.isInteger(seconds) || seconds < 1 || seconds > 86400) {
            return new Response(JSON.stringify({ error: 'Invalid time' }), { status: 400, headers: corsHeaders });
        }

        const raw    = await env.GAUNTLET_SCORES.get(kvKey);
        const scores = raw ? JSON.parse(raw) : [];

        const entry = {
            name:    name.trim().slice(0, 20),
            correct,
            seconds,
            date:    now.toISOString().slice(0, 10),
        };

        scores.push(entry);
        // Sort: most correct first; ties broken by fastest time
        scores.sort((a, b) => b.correct - a.correct || a.seconds - b.seconds);
        const top10 = scores.slice(0, 10);

        await env.GAUNTLET_SCORES.put(kvKey, JSON.stringify(top10));

        const rank = top10.indexOf(entry) + 1;
        return new Response(
            JSON.stringify({ rank: rank > 0 ? rank : null, month, scores: top10 }),
            { headers: corsHeaders }
        );
    }

    return new Response('Method not allowed', { status: 405, headers: corsHeaders });
}
