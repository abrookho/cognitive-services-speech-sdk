//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
using System.Globalization;
namespace Microsoft.CognitiveServices.Speech.Intent
{
    /// <summary>
    /// Define payload of intent intermediate/final result events.
    /// </summary>
    public sealed class IntentRecognitionResultEventArgs : System.EventArgs
    {
        internal IntentRecognitionResultEventArgs(Internal.IntentRecognitionEventArgs e)
        {
            eventArgImpl = e;
            Result = new IntentRecognitionResult(e.Result);
            SessionId = e.SessionId;
        }

        /// <summary>
        /// Represents the intent recognition result.
        /// </summary>
        public IntentRecognitionResult Result { get; }

        /// <summary>
        /// A String represents the session identifier.
        /// </summary>
        public string SessionId { get; }

        /// <summary>
        /// Returns a string that represents the session id and the intent recognition result event.
        /// </summary>
        /// <returns>A string that represents the intent recognition result event.</returns>
        public override string ToString()
        {
            return string.Format(CultureInfo.InvariantCulture, "SessionId:{0} ResultId:{1} Status:{2} IntentId:<{3}> Recognized text:<{4}>.", 
                SessionId, Result.ResultId, Result.RecognitionStatus, Result.IntentId, Result.Text);
        }

        // Hold the reference.
        private Internal.IntentRecognitionEventArgs eventArgImpl;
    }
}