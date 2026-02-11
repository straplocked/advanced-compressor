#include "EnvelopeFollower.h"

void EnvelopeFollower::prepare (double sampleRate)
{
    currentSampleRate = sampleRate;
    reset();
    updateCoefficients();
}

void EnvelopeFollower::reset()
{
    currentGR = 0.0f;
    holdCounter = 0;
    prevTargetGR = 0.0f;
    optoRelease1 = 0.0f;
    optoRelease2 = 0.0f;
}

void EnvelopeFollower::setStyle (CompressorStyle style)
{
    currentStyle = style;
    behavior = getStyleBehavior (style);
    updateCoefficients();
}

void EnvelopeFollower::setAttack (float attackMs)
{
    userAttackMs = attackMs;
    updateCoefficients();
}

void EnvelopeFollower::setRelease (float releaseMs)
{
    userReleaseMs = releaseMs;
    updateCoefficients();
}

void EnvelopeFollower::setHold (float holdMs)
{
    userHoldMs = holdMs;
    updateCoefficients();
}

void EnvelopeFollower::updateCoefficients()
{
    float effectiveAttack = userAttackMs * behavior.attackScale;
    float effectiveRelease = userReleaseMs * behavior.releaseScale;
    float effectiveHold = userHoldMs * behavior.holdScale;

    attackCoeff = CompressorMath::timeToCoeff (effectiveAttack, currentSampleRate);
    releaseCoeff = CompressorMath::timeToCoeff (effectiveRelease, currentSampleRate);
    holdSamples = CompressorMath::msToSamples (effectiveHold, currentSampleRate);

    // Opto two-stage release coefficients
    if (currentStyle == CompressorStyle::Opto)
    {
        optoRelease1 = CompressorMath::timeToCoeff (effectiveRelease * 0.3f, currentSampleRate);
        optoRelease2 = CompressorMath::timeToCoeff (effectiveRelease * 2.0f, currentSampleRate);
    }

    // Op-El gain-dependent attack coefficients
    if (behavior.gainDepAttack)
    {
        opElFastAttack = CompressorMath::timeToCoeff (effectiveAttack * 0.2f, currentSampleRate);
        opElSlowAttack = CompressorMath::timeToCoeff (effectiveAttack * 3.0f, currentSampleRate);
    }
}

float EnvelopeFollower::processSample (float targetGR)
{
    // targetGR is negative (gain reduction)
    // More negative = more compression

    if (targetGR < currentGR)
    {
        // Attack: GR is increasing (going more negative)
        holdCounter = holdSamples;

        if (behavior.gainDepAttack)
        {
            // Op-El: attack speed depends on GR depth -- more GR = slower attack
            float grDepth = std::abs (targetGR);
            float blend = std::min (grDepth / 15.0f, 1.0f);
            float coeff = opElFastAttack * (1.0f - blend) + opElSlowAttack * blend;
            currentGR = coeff * currentGR + (1.0f - coeff) * targetGR;
        }
        else
        {
            currentGR = attackCoeff * currentGR + (1.0f - attackCoeff) * targetGR;
        }
    }
    else
    {
        // Release phase
        if (holdCounter > 0)
        {
            --holdCounter;
            // During hold, maintain current GR
        }
        else
        {
            if (currentStyle == CompressorStyle::Opto)
            {
                // Opto two-stage release: fast initial, then slow
                float grDepth = std::abs (currentGR);
                float blend = std::min (grDepth / 20.0f, 1.0f);
                float coeff = optoRelease1 * (1.0f - blend) + optoRelease2 * blend;
                currentGR = coeff * currentGR + (1.0f - coeff) * targetGR;
            }
            else if (behavior.programDepRelease)
            {
                // Program-dependent: faster release for transients, slower for sustained
                float grDepth = std::abs (currentGR);
                float releaseSpeed = 1.0f + grDepth * 0.05f;
                float adaptedRelease = userReleaseMs * behavior.releaseScale / releaseSpeed;
                float adaptedCoeff = CompressorMath::timeToCoeff (adaptedRelease, currentSampleRate);
                currentGR = adaptedCoeff * currentGR + (1.0f - adaptedCoeff) * targetGR;
            }
            else
            {
                currentGR = releaseCoeff * currentGR + (1.0f - releaseCoeff) * targetGR;
            }
        }
    }

    prevTargetGR = targetGR;
    return currentGR;
}
